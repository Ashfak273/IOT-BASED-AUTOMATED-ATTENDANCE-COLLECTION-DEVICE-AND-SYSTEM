#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <Adafruit_Fingerprint.h>
#include <HardwareSerial.h>
#include <WiFi.h>

#include <Keypad.h>

String NAME;
String ID;
String Event_Name = "EC 6040";
String Key = "VmJDoz4WcT6PO0kaFBKlQ";
// Making IFTTT URL resource
String resource = "/trigger/" + Event_Name + "/with/key/" + Key;
// server where the HTTP requests  sent to trigger IFTTT events.
const char* server = "maker.ifttt.com";
// Replace with your SSID and Password

const char* ssid     = "AK";
const char* password = "mzashfak.273";

// const char* ssid     = "It's me";
// const char* password = "Ahsan123";

// const char* ssid     = "Dialog 4G";
// const char* password = "@Bismi1996@";

// const char* ssid     = "IABWSC";
// const char* password = "Ahsanasc";


//Keypad
const byte ROWS = 4;
const byte COLS = 4;

char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {13, 12, 14, 27};
byte colPins[COLS] = {26, 25, 33, 32};

Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

//display,finger
Adafruit_SSD1306 display(128, 64, &Wire, -1);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&Serial2);

uint16_t id;


//Declaring below mentioned functions 
uint16_t readnumber(void);
uint16_t getFingerprintEnroll();





void setup() 
{
  // FOR oled dISPLAY
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  
  //OTHER CODE
  Serial.begin(9600);
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(100);


  // set the data rate for the sensor serial port
  finger.begin(57600);
  delay(5);
  if (finger.verifyPassword()) 
  {
    display.setCursor(0, 0);
    display.print("Found the sensor");
    display.display();
    delay(10);
  } 
  else 
  {
    display.setCursor(0, 0);
    display.print("no fingerprint sensor");
    display.display();
    delay(1500);
    while (1) 
    { 
      delay(1); 
    }
  }

  Serial.println(F("\n\nReading sensor parameters"));
  finger.getParameters();
  Serial.print(F("Status: 0x")); Serial.println(finger.status_reg, HEX);
  Serial.print(F("Sys ID: 0x")); Serial.println(finger.system_id, HEX);
  Serial.print(F("Capacity: ")); Serial.println(finger.capacity);
  Serial.print(F("Security level: ")); Serial.println(finger.security_level);
  Serial.print(F("Device address: ")); Serial.println(finger.device_addr, HEX);
  Serial.print(F("Packet len: ")); Serial.println(finger.packet_len);
  Serial.print(F("Baud rate: ")); Serial.println(finger.baud_rate);

}


 
void loop() 
{
  display.setCursor(0, 18);
  display.print("Enroll : press A\n\nVerify : press B");
  display.display();
  display.setCursor(0, 48);
  display.print("Press the button");
  display.display(); 
   
  // char custom = customKeypad.getKey();
  // if (custom)
  // {
  //   if (custom == 'A') 
  //   {
  //     enroll();
  //   } 
  //   else if (custom == 'B') 
  //   {
  //     verify();
  //   }    
  // } 

  while (1) 
  {
    char customKey = customKeypad.getKey();

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




//This code run for 1000 times once activated untill reset pressed
void verify()
{
  finger.getTemplateCount();
  if (finger.templateCount == 0) 
  {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("no Data : Plz Enroll");
    display.display();
    delay(1000);
    display.setCursor(0, 12);
    display.print("Directing to Enroll");
    display.display();
    delay(2000);
    enroll();

  }
  else 
  {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("It has: ");
    display.print(finger.templateCount);display.print(" templates");
    display.display();
    delay(500);

    display.setCursor(0, 12);
    display.print("Connecting: ");display.print(ssid);
    display.display();
    WiFi.begin(ssid, password);
    delay(100);

    int timeout = 10 * 4; // 40 seconds
    while (WiFi.status() != WL_CONNECTED  && (timeout-- > 0)) 
    {
      delay(250);        
    }
    if (WiFi.status() == WL_CONNECTED) 
    {                
      display.setCursor(0, 24);
      display.print("Connected to wifi");
      display.display();
      delay(500);
    }
    else 
    {
      display.setCursor(0, 24);
      display.print("Failed to connect");
      display.display();
      delay(1000);
      verify();               
    }         
    display.setCursor(0, 36);
    display.print("WiFi IP:");display.print(WiFi.localIP());
    display.display();
    Serial.print(millis());
    delay(1500);
  }
  //break;
  
  for (int i = 0; i < 1000; i++)
  { 
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Place the finger");
    display.display();
    getFingerprintID();
    delay(50);  
  } 
}


//Codes for the verification Start---------------------------------------
uint8_t getFingerprintID() 
{
  uint8_t p = finger.getImage();
  switch (p) 
  {
    case FINGERPRINT_OK:
      display.setCursor(0, 12);
      display.print("Image taken");
      display.display();
      delay(100);
      break;
    case FINGERPRINT_NOFINGER:
      display.setCursor(0, 12);
      display.print("No finger detected");
      display.display();
      delay(1000);
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      display.setCursor(0, 12);
      display.print("Communication Error");
      display.display();
      delay(2000);
      return p;
    case FINGERPRINT_IMAGEFAIL:
      display.setCursor(0, 12);
      display.print("Imaging Error");
      display.display();
      delay(2000);
      return p;
    default:
      display.setCursor(0, 12);
      display.print("Error");
      display.display();
      delay(2000);
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      display.setCursor(0, 24);
      display.print("Image converted");
      display.display();
      delay(100);
      break;
    case FINGERPRINT_IMAGEMESS:
      display.setCursor(0, 24);
      display.print("Error : too messy");
      display.display();
      delay(2000);
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      display.setCursor(0, 24);
      display.print("Communication Error");
      display.display();
      delay(2000);
      return p;
    case FINGERPRINT_FEATUREFAIL:
      display.setCursor(0, 24);
      display.print("Error: Nofeatures");
      display.display();
      delay(2000);
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      display.setCursor(0, 24);
      display.print("Error: Nofeatures");
      display.display();
      delay(2000);
      return p;
    default:
      display.setCursor(0, 24);
      display.print("Error");
      display.display();
      delay(2000);
      return p;
  }

  // OK converted!
  p = finger.fingerSearch();
  if (p == FINGERPRINT_OK) 
  {
    display.setCursor(0, 36);
    display.print("Found a match!");
    display.display();
    delay(300);
  } 
  else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    
    display.setCursor(0, 36);
    display.print("Communication Error");
    display.display();
    delay(2000);
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {

    display.setCursor(0, 36);
    display.print("Error : No match");
    display.display();
    delay(2000);
    return p;
  } else {
    display.setCursor(0, 36);
    display.print("Error");
    display.display();
    delay(2000);
    return p;
  }

  // found a match!
  display.setCursor(0, 48);
  display.print("Found ID #");
  display.print(finger.fingerID);
  display.display();
  delay(1100);

  //saving Re number in Google sheet
  for (int fingerID = 1; fingerID <= 1000; fingerID++) 
  {
    if (finger.fingerID == fingerID) 
    {      
      // Format ID with leading zeros based on the value
      if (fingerID <= 9) 
      {
        ID = "00" + String(fingerID);
      } 
      else if (fingerID > 9 && fingerID <= 99) 
      { 
        ID = "0" + String(fingerID);
      }
      else 
      {
        ID = String(fingerID);
      }
      NAME = "2019/E/" + ID;
      if (finger.confidence >= 50) 
      {
        display.clearDisplay();
        display.setCursor(0, 14);
        display.print("Marked : "); display.println(NAME);
        delay(100);
        display.setCursor(0, 28);
        display.print("remove finger");
        display.display();
        makeIFTTTRequest();
        delay(3000);
      }
      // No break statement here; the loop will continue checking other IDs
    }
  }
  return finger.fingerID;
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;

  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID);
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  return finger.fingerID;
}

// Make an HTTP request to the IFTTT web service
void makeIFTTTRequest() 
{
  WiFiClient client;
  int retries = 10;
  while (!!!client.connect(server, 80) && (retries-- > 0)) 
  {
    Serial.print(".");
  }

  Serial.println();
  if (!!!client.connected()) 
  {
    display.setCursor(0, 42);
    display.print("Failed to send...Retry");
    display.display();
    Serial.println("Failed to connect...");
  }

  Serial.print("Request resource: ");
  Serial.println(resource);

  String jsonObject = String("{\"value1\":\"") + NAME + "\",\"value2\":\"" + ID
                      + "\"}";

  client.println(String("POST ") + resource + " HTTP/1.1");
  client.println(String("Host: ") + server);
  client.println("Connection: close\r\nContent-Type: application/json");
  client.print("Content-Length: ");
  client.println(jsonObject.length());
  client.println();
  client.println(jsonObject);

  int timeout = 5 * 10; // 5 seconds
  while (!!!client.available() && (timeout-- > 0)) 
  {
    delay(100);
  }
  if (!!!client.available()) 
  {
    Serial.println("No response...");
  }
  while (client.available()) 
  {
    Serial.write(client.read());
  }
  Serial.println("\nclosing connection");
  client.stop();
}

//Codes for the verification End---------------------------------------






//Codes for the Enrollment Start---------------------------------------

//This code run for 200 times once activated untill reset pressed
void enroll()
{
  for (int i = 0; i < 200; i++) 
  { 
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Ready to enroll!");

    display.setCursor(0, 15);
    display.println("type ID,press C#");
    display.display();

    id = readnumber();
    if (id == 0)  // ID #0 not allowed, try again!
    {
      return;
    }

    // Only display "Enrolling ID #" if a valid ID is entered
    display.setCursor(0, 30);
    display.print("Enrolling ID #");
    display.print(id);
    display.display();

    while (!getFingerprintEnroll()); // Continue this loop until fingerprint enrollment is successful
  }
  //break;
  delay(500); // Debounce delay
}



uint16_t readnumber(void) 
{
  uint16_t num = 0;
  int digits = 0;
  int xOffset = 100;
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
      display.setCursor(xOffset, 15);
      display.print(customKey);
      display.display();

      xOffset += 8;  

      if (digits >= 4) 
      {
        // Reset x-coordinate for the next number
        xOffset = 100;
      }
    }
    else if (customKey == 'D')
    {
      display.clearDisplay();
      loop();
    }
  }
}


uint16_t getFingerprintEnroll() 
{
  int p = -1;

  display.setCursor(0, 44);
  display.print("Place the finger");
  display.display();

  //display.display();
  while (p != FINGERPRINT_OK) 
  {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:

      display.setCursor(0, 55);
      display.print("Image taken");
      display.display();
      delay(500);

      break;
    case FINGERPRINT_NOFINGER:
      Serial.println(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      display.setCursor(0, 55);
      display.print("Communication error");
      display.display();
      delay(1000);

      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      display.setCursor(0, 55);
      display.print("Imaging error");
      display.display();
      delay(1000);
      break;
    default:
      display.setCursor(0, 55);
      display.print("Unknown error");
      display.display();
      delay(1000);
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
      break;
    case FINGERPRINT_IMAGEMESS:
      display.clearDisplay();
      display.setCursor(0, 0);
      display.print("Image too messy");
      display.display();
      delay(1000);
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      display.clearDisplay();
      display.setCursor(0, 0);
      display.print("Communication error");
      display.display();
      delay(1000);
      return p;
    case FINGERPRINT_FEATUREFAIL:
      display.clearDisplay();
      display.setCursor(0, 0);
      display.print("Error:not find finger");
      display.display();
      delay(1000);
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      display.clearDisplay();
      display.setCursor(0, 0);
      display.print("Error:not find finger");
      display.display();
      delay(1000);
      return p;
    default:
      display.clearDisplay();
      display.setCursor(0, 0);
      display.print("Unknown error Error");
      display.display();
      delay(1000);
      return p;
  }


  display.setCursor(0, 15);
  display.print("Remove finger");
  display.display();
  delay(1000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) 
  {
    p = finger.getImage();
  }

  display.setCursor(0, 30);
  display.print("ID #");
  display.print(id);
  display.display();

  Serial.print("ID "); Serial.println(id);
  p = -1;

  display.setCursor(0, 44);
  display.print("Place same finger");
  display.display();
  
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
      display.setCursor(0, 55);
      display.print("Communication error");
      display.display();
      delay(1000);
      break;
    case FINGERPRINT_IMAGEFAIL:
      display.setCursor(0, 55);
      display.print("Imaging error");
      display.display();
      delay(1000);
      break;
    default:
      display.setCursor(0, 55);
      display.print("Unknown error");
      display.display();
      delay(1000);
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:

      display.setCursor(0, 55);
      display.print("Image converted");
      display.display();
      delay(500);
      break;
    case FINGERPRINT_IMAGEMESS:
      display.setCursor(0, 55);
      display.print("Error:Image too messy");
      display.display();
      delay(1000);
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      display.setCursor(0, 55);
      display.print("Communication error");
      display.display();
      delay(1000);
      return p;
    case FINGERPRINT_FEATUREFAIL:
      display.setCursor(0, 55);
      display.print("Error:not find finger");
      display.display();
      delay(1000);
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      display.setCursor(0, 55);
      display.print("Error:not find finger");
      display.display();
      delay(1000);
      return p;
    default:
      display.setCursor(0, 55);
      display.print("Unknown error");
      display.display();
      delay(1000);
      return p;
  }

  // OK converted!

  p = finger.createModel();
  if (p == FINGERPRINT_OK) {

    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Prints matched!");
    display.display();
    delay(500);

  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Communication error");
    display.display();
    delay(1000);
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {

    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Finger not match");
    display.display();
    delay(1000);
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }


  display.setCursor(0, 15);
  display.print("ID #");
  display.print(id);
  display.display();


  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {

    display.setCursor(0, 30);
    display.print("Stored!");
    display.display();
    delay(1500);

    display.setCursor(0, 44);
    display.print("Enroll more ;");
    display.display();
    delay(1000);

  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    display.setCursor(0, 30);
    display.print("Communication error");
    display.display();
    delay(1000);
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    display.setCursor(0, 30);
    display.print("Error: can't store");
    display.display();
    delay(1000);
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    display.setCursor(0, 30);
    display.print("Error:writing to flash");
    display.display();
    delay(1000);
    return p;
  } else {
    display.setCursor(0, 30);
    display.print("Unknown error");
    display.display();
    delay(1000);
    return p;
  }

  return true;
}

