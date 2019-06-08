#include <SPI.h>
#include <MFRC522.h>

#include <ESP8266WiFi.h>

#define RST_PIN         D3          // Configurable, see typical pin layout above
#define SS_PIN          D8         // Configurable, see typical pin layout above

// LEDs config
#define WHITE_LED D1
#define BUZZER D2

#define PIN_ON HIGH
#define PIN_OFF LOW

// WiFi configuration

// Replace WIFINAME with WiFi network name
// Replace WIFIPWD with WiFi network password
const char* ssid = "WIFINAME";
const char* password = "WIFIPWD";

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

void setup() {
  Serial.begin(115200);   // Initialize serial communications with the PC
  while (!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  SPI.begin();      // Init SPI bus
  mfrc522.PCD_Init();   // Init MFRC522
  mfrc522.PCD_DumpVersionToSerial();  // Show details of PCD - MFRC522 Card Reader details
  //Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));

  Serial.println("Configuring LEDs and the buzzer...");
  pinMode(BUZZER, OUTPUT);
  pinMode(WHITE_LED, OUTPUT);
  
  Serial.print("Connecting to ");
  Serial.print(ssid);
  Serial.println("...");

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println('\n');

  // Successful connection
  Serial.println("Connection established!");
  Serial.print("IP:\t");
  Serial.println(WiFi.localIP());

}

void loop() {
  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  // Dump debug info about the card; PICC_HaltA() is automatically called
  mfrc522.PICC_DumpToSerial(&(mfrc522.uid));

  digitalWrite(BUZZER, PIN_ON);
  digitalWrite(WHITE_LED, PIN_ON);
  delay(750);
  digitalWrite(BUZZER, PIN_OFF);
  digitalWrite(WHITE_LED, PIN_OFF);
}
