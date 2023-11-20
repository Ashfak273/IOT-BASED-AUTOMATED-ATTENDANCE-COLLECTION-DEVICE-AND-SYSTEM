

#include <Keypad.h>

const byte ROWS = 4; // Four rows
const byte COLS = 4; // Three columns

char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {13, 12, 14, 27}; // Connect to the row pinouts of the keypad
byte colPins[COLS] = {26, 25, 33, 32}; // Connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS );

void setup(){
  Serial.begin(9600);
}

void loop(){
  char key = keypad.getKey();
  
  if (key){
    Serial.println(key);
  }
}
