
#include <Keypad.h>

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns

uint16_t id;
uint16_t readnumber(void);
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {13, 12, 14, 27}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {26, 25, 33, 32}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

bool shouldReset = false; // Flag to indicate if a reset is needed

void setup()
{
  Serial.begin(9600);
}

void enroll()
{
  for (int i = 0; i < 200; i++) 
  { 
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Ready to enroll!");

    display.setCursor(0, 12);
    display.println("type ID #");
    display.display();

    Serial.println("Ready to enroll a fingerprint!");
    Serial.println("Please type in the ID # you want to save this finger as...");
    id = readnumber();
    if (id == 0)  // ID #0 not allowed, try again!
    {
      return;
    }

    // Only display "Enrolling ID #" if a valid ID is entered
    display.setCursor(0, 24);
    display.print("Enrolling ID #");
    display.print(id);
    display.display();

    Serial.print("Enrolling ID #");
    Serial.println(id);

    while (!getFingerprintEnroll()); // Continue this loop until fingerprint enrollment is successful
  }
  //break;
  delay(500); // Debounce delay
}


void verify() 
{
  // Your verify function code here
  Serial.println("Verify function called");
}

void resetCode() 
{
  Serial.println("Resetting code...");
  delay(1000); // Delay to allow time for the user to release the 'D' key
  shouldReset = false; // Reset the flag
  loop(); // Re-run the loop function to reset the code
}

void loop()
{
  if (shouldReset) 
  {
    resetCode();
  } 
  else 
  {
    char customKey = customKeypad.getKey();    
    if (customKey)
    {
      // Check which key was pressed and call the corresponding function
      if (customKey == 'A') 
      {
        enroll();
      } 
      else if (customKey == 'B') 
      {
        verify();
      }
      else if (customKey == 'D') 
      {
        shouldReset = true; // Set the flag to trigger a reset
      }
    }
  }
}
