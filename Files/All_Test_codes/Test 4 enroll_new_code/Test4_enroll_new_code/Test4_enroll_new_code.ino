#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <Adafruit_Fingerprint.h>
#include <HardwareSerial.h>

Adafruit_SSD1306 display(128, 64, &Wire, -1);

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&Serial2);

uint16_t id;


void setup()
{
  // FOR oled dISPLAY
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();

  //OTHER CODE
  Serial.begin(9600);
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(100);
  Serial.println("\n\nAdafruit Fingerprint sensor enrollment");

  // set the data rate for the sensor serial port
  finger.begin(57600);

  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); }
  }
}

uint16_t readnumber(void){
  uint16_t num = 0;
 
  while (num == 0) {
    while (! Serial.available());
    num = Serial.parseInt();
  }
  return num;
}



void loop()                     // run over and over again
{
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Ready to enroll!");

  display.setCursor(0, 12);
  display.println("type ID #");
  display.display();

  Serial.println("Ready to enroll a fingerprint!");
  Serial.println("Please type in the ID # you want to save this finger as...");
  id = readnumber();
  if (id == 0) {// ID #0 not allowed, try again!
    return;
  }

  // Only display "Enrolling ID #" if a valid ID is entered
  display.setCursor(0, 24);
  display.print("Enrolling ID #");
  display.print(id);
  display.display();

  Serial.print("Enrolling ID #");
  Serial.println(id);

  while (!  getFingerprintEnroll() );
}

uint16_t getFingerprintEnroll() {

  int p = -1;

  display.setCursor(0, 36);
  display.print("Place the finger");
  display.display();

  //display.display();
  Serial.print("Waiting for valid finger to enroll as #"); Serial.println(id);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:

      display.setCursor(0, 48);
      display.print("Image taken");
      display.display();
      delay(500);

      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      display.setCursor(0, 48);
      display.print("Communication error");
      display.display();
      delay(1000);

      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      display.setCursor(0, 48);
      display.print("Imaging error");
      display.display();
      delay(1000);
      Serial.println("Imaging error");
      break;
    default:
      display.setCursor(0, 48);
      display.print("Unknown error");
      display.display();
      delay(1000);
      Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:

      display.clearDisplay();
      display.setCursor(0, 0);
      display.print("Image converted");
      display.display();

      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      display.clearDisplay();
      display.setCursor(0, 0);
      display.print("Image too messy");
      display.display();
      delay(1000);
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      display.clearDisplay();
      display.setCursor(0, 0);
      display.print("Communication error");
      display.display();
      delay(1000);
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      display.clearDisplay();
      display.setCursor(0, 0);
      display.print("Error:not find finger");
      display.display();
      delay(1000);
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      display.clearDisplay();
      display.setCursor(0, 0);
      display.print("Error:not find finger");
      display.display();
      delay(1000);
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      display.clearDisplay();
      display.setCursor(0, 0);
      display.print("Unknown error Error");
      display.display();
      delay(1000);
      Serial.println("Unknown error");
      return p;
  }


  display.setCursor(0, 12);
  display.print("Remove finger");
  display.display();

  Serial.println("Remove finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }

  display.setCursor(0, 24);
  display.print("ID #");
  display.print(id);
  display.display();

  Serial.print("ID "); Serial.println(id);
  p = -1;

  display.setCursor(0, 36);
  display.print("Place same finger");
  display.display();
  
  Serial.println("Place same finger again");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.print(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      display.setCursor(0, 48);
      display.print("Communication error");
      display.display();
      delay(1000);
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      display.setCursor(0, 48);
      display.print("Imaging error");
      display.display();
      delay(1000);
      Serial.println("Imaging error");
      break;
    default:
      display.setCursor(0, 48);
      display.print("Unknown error");
      display.display();
      delay(1000);
      Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:

      display.setCursor(0, 48);
      display.print("Image converted");
      display.display();
      delay(500);

      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      display.setCursor(0, 48);
      display.print("Error:Image too messy");
      display.display();
      delay(1000);
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      display.setCursor(0, 48);
      display.print("Communication error");
      display.display();
      delay(1000);
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      display.setCursor(0, 48);
      display.print("Error:not find finger");
      display.display();
      delay(1000);
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      display.setCursor(0, 48);
      display.print("Error:not find finger");
      display.display();
      delay(1000);
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      display.setCursor(0, 48);
      display.print("Unknown error");
      display.display();
      delay(1000);
      Serial.println("Unknown error");
      return p;
  }

  // OK converted!
  Serial.print("Creating model for #");  Serial.println(id);

  p = finger.createModel();
  if (p == FINGERPRINT_OK) {

    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Prints matched!");
    display.display();
    delay(500);

    Serial.println("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Communication error");
    display.display();
    delay(1000);
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {

    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Finger not match");
    display.display();
    delay(1000);
    Serial.println("Fingerprints did not match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }


  display.setCursor(0, 12);
  display.print("ID #");
  display.print(id);
  display.display();

  Serial.print("ID "); Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {

    display.setCursor(0, 24);
    display.print("Stored!");
    display.display();
    Serial.println("Stored!");
    delay(3000);

    display.setCursor(0, 36);
    display.print("Enroll more ;");
    display.display();
    delay(1000);

  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    display.setCursor(0, 24);
    display.print("Communication error");
    display.display();
    delay(1000);
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    display.setCursor(0, 24);
    display.print("Error: can't store");
    display.display();
    delay(1000);
    Serial.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    display.setCursor(0, 24);
    display.print("Error:writing to flash");
    display.display();
    delay(1000);
    Serial.println("Error writing to flash");
    return p;
  } else {
    display.setCursor(0, 24);
    display.print("Unknown error");
    display.display();
    delay(1000);
    Serial.println("Unknown error");
    return p;
  }

  return true;
}