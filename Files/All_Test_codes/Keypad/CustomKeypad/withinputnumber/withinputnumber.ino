#include <Keypad.h>

const byte ROWS = 4;
const byte COLS = 4;

uint16_t id;
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {13, 12, 14, 27};
byte colPins[COLS] = {26, 25, 33, 32};

Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);


void setup()
{
  Serial.begin(9600);
}


uint16_t readnumber(void) 
{
  uint16_t num = 0;
  int digits = 0;
  while (1) 
  {
    char customKey = customKeypad.getKey();

    if (customKey == 'C' && digits >= 1) 
    {
      return num;
    } 
    else if (customKey >= '0' && customKey <= '9' && digits < 4) 
    {
      num = num * 10 + (customKey - '0');
      digits++;
      // Optionally, you can display the entered digits on an LCD or Serial Monitor
      Serial.print(customKey);
    }
  }
}

void enroll() 
{
  for (int i = 0; i < 200; i++) {
    // ... (rest of your enroll function code)

    // Prompt the user to enter a number and store it in 'id'
    Serial.println("Please enter a number (1-4 digits) and press 'C' to save:");
    id = readnumber();
    if (id == 0) {
      return; // ID #0 not allowed, try again!
    }

    // ... (rest of your enroll function code)
  }

  delay(500); // Debounce delay
}

void verify() {
  // Your verify function code here
  Serial.println("Verify function called");
}


void loop() 
{
  char customKey = customKeypad.getKey();
  if (customKey) 
  {
    if (customKey == 'A') 
    {
      enroll();
    } 
    else if (customKey == 'B') 
    {
      verify();
    }       
  }  
}
