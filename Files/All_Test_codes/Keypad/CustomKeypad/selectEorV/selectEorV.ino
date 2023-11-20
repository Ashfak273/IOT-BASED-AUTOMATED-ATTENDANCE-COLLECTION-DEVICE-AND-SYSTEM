
#include <Keypad.h>

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns

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

void setup(){
  Serial.begin(9600);
}

void enroll() {
  // Your enroll function code here
  Serial.println("Enroll function called");
}

void verify() {
  // Your verify function code here
  Serial.println("Verify function called");
}
  
void loop(){
  char customKey = customKeypad.getKey();
  
  if (customKey){
    // Check which key was pressed and call the corresponding function
    if (customKey == 'A') {
      enroll();
    } else if (customKey == 'B') {
      verify();
    }
  }
}
