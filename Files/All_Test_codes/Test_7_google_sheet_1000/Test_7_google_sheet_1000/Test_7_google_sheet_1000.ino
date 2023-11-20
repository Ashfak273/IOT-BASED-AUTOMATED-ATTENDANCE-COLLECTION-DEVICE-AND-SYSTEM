#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <Adafruit_Fingerprint.h>
#include <HardwareSerial.h>
#include <WiFi.h>

String NAME;
String ID;
String Event_Name = "EC 6040";
String Key = "VmJDoz4WcT6PO0kaFBKlQ";
// Making IFTTT URL resource
String resource = "/trigger/" + Event_Name + "/with/key/" + Key;
// server where the HTTP requests  sent to trigger IFTTT events.
const char* server = "maker.ifttt.com";
// Replace with your SSID and Password
const char* ssid     = "It's me";
const char* password = "Ahsan123";


Adafruit_SSD1306 display(128, 64, &Wire, -1);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&Serial2);

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
  Serial.println("\n\nAdafruit finger detect test");

  // set the data rate for the sensor serial port
  finger.begin(57600);
  delay(5);
  if (finger.verifyPassword()) {
    display.setCursor(0, 0);
    display.print("Found the sensor");
    display.display();
    delay(10);
    Serial.println("Found fingerprint sensor!");
  } else {
    display.setCursor(0, 0);
    display.print("no fingerprint sensor");
    display.display();
    delay(1500);
    Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); }
  }

  Serial.println(F("Reading sensor parameters"));
  finger.getParameters();
  Serial.print(F("Status: 0x")); Serial.println(finger.status_reg, HEX);
  Serial.print(F("Sys ID: 0x")); Serial.println(finger.system_id, HEX);
  Serial.print(F("Capacity: ")); Serial.println(finger.capacity);
  Serial.print(F("Security level: ")); Serial.println(finger.security_level);
  Serial.print(F("Device address: ")); Serial.println(finger.device_addr, HEX);
  Serial.print(F("Packet len: ")); Serial.println(finger.packet_len);
  Serial.print(F("Baud rate: ")); Serial.println(finger.baud_rate);

  finger.getTemplateCount();
  if (finger.templateCount == 0) {
    display.setCursor(0, 12);
    display.print("no Data : Plz Enroll");
    display.display();
    delay(1500);
    Serial.print("Sensor doesn't contain any fingerprint data. Please run the 'enroll' example.");
  }
  else {
    finger.getTemplateCount();
    display.setCursor(0, 12);
    display.print("It has: ");
    display.print(finger.templateCount);display.print(" templates");
    display.display();
    Serial.println("Waiting for valid finger...");
    Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
    delay(500);

    display.setCursor(0, 24);
    display.print("Connecting: ");display.print(ssid);
    display.display();
    Serial.print("Connecting to: ");
    Serial.print(ssid);
    WiFi.begin(ssid, password);
    delay(100);

    int timeout = 10 * 4; // 10 seconds
    while (WiFi.status() != WL_CONNECTED  && (timeout-- > 0)) {
      delay(250);
      Serial.print(".");  //untill get connection
    }
    if (WiFi.status() == WL_CONNECTED) {
      
      display.setCursor(0, 36);
      display.print("Connected to wifi");
      display.display();
      Serial.println("Connected"); 
      delay(500);
    }
    else {
      display.setCursor(0, 36);
      display.print("Failed to connect");
      display.display();
      Serial.println("Failed to connect, going back to sleep");
      delay(1500);
      exit(0);
    }
    display.setCursor(0, 48);
    display.print("WiFi IP:");display.print(WiFi.localIP());
    display.display();
    Serial.print("WiFi connected in: ");
    Serial.print(millis());
    Serial.print(", IP address: ");
    Serial.println(WiFi.localIP());
    delay(2000);
  }
}



void loop()                     // run over and over again
{ 
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Place the finger");
  display.display();
  
  getFingerprintID();
  delay(50);            //don't ned to run this at full speed.
}

uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      display.setCursor(0, 12);
      display.print("Image taken");
      display.display();
      delay(100);

      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      display.setCursor(0, 12);
      display.print("No finger detected");
      display.display();
      delay(1000);

      Serial.println("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      display.setCursor(0, 12);
      display.print("Communication Error");
      display.display();
      delay(2000);

      Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      display.setCursor(0, 12);
      display.print("Imaging Error");
      display.display();
      delay(2000);

      Serial.println("Imaging error");
      return p;
    default:
      display.setCursor(0, 12);
      display.print("Error");
      display.display();
      delay(2000);

      Serial.println("Unknown error");
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

      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      display.setCursor(0, 24);
      display.print("Error : too messy");
      display.display();
      delay(2000);

      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      display.setCursor(0, 24);
      display.print("Communication Error");
      display.display();
      delay(2000);

      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      display.setCursor(0, 24);
      display.print("Error: Nofeatures");
      display.display();
      delay(2000);

      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      display.setCursor(0, 24);
      display.print("Error: Nofeatures");
      display.display();
      delay(2000);

      Serial.println("Could not find fingerprint features");
      return p;
    default:
      display.setCursor(0, 24);
      display.print("Error");
      display.display();
      delay(2000);

      Serial.println("Unknown error");
      return p;
  }

  // OK converted!
  p = finger.fingerSearch();
  if (p == FINGERPRINT_OK) {
    display.setCursor(0, 36);
    display.print("Found a match!");
    display.display();
    delay(100);
    
    Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    
    display.setCursor(0, 36);
    display.print("Communication Error");
    display.display();
    delay(2000);
  
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {

    display.setCursor(0, 36);
    display.print("Error : No match");
    display.display();
    delay(2000);

    Serial.println("Did not find a match");
    return p;
  } else {
    display.setCursor(0, 36);
    display.print("Error");
    display.display();
    delay(2000);

    Serial.println("Unknown error");
    return p;
  }

  // found a match!
  display.setCursor(0, 48);
  display.print("Found ID #");
  display.print(finger.fingerID);
  display.display();
  
  Serial.print("Found ID #"); Serial.print(finger.fingerID);
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  delay(1500);

  //saving Re number in Google sheet
  for (int fingerID = 1; fingerID <= 1000; fingerID++) {
    if (finger.fingerID == fingerID) {
      Serial.print("!!--");
      Serial.println(finger.fingerID);
      
      // Format ID with leading zeros based on the value
      if (fingerID <= 9) {
        ID = "00" + String(fingerID);
      } 
      else if (fingerID > 9 && fingerID <= 99) { 
        ID = "0" + String(fingerID);
      }
      else {
        ID = String(fingerID);
      }
      NAME = "2019/E/" + ID;
      if (finger.confidence >= 50) {

        display.clearDisplay();
        display.setCursor(0, 14);
        display.print("Marked : "); display.println(NAME);
        delay(100);
        display.setCursor(0, 36);
        display.print("remove finger");
        display.display();
        
        Serial.print("Attendance Marked for ");
        Serial.println(NAME);
        makeIFTTTRequest();
        delay(2500);
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
void makeIFTTTRequest() {
  Serial.print("Connecting to ");
  Serial.print(server);

  WiFiClient client;
  int retries = 5;
  while (!!!client.connect(server, 80) && (retries-- > 0)) {
    Serial.print(".");
  }

  Serial.println();
  if (!!!client.connected()) {
    Serial.println("Failed to connect...");
  }

  Serial.print("Request resource: ");
  Serial.println(resource);

  // Temperature in Celsius
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
  while (!!!client.available() && (timeout-- > 0)) {
    delay(100);
  }
  if (!!!client.available()) {
    Serial.println("No response...");
  }
  while (client.available()) {
    Serial.write(client.read());
  }
  Serial.println("\nclosing connection");
  client.stop();
}