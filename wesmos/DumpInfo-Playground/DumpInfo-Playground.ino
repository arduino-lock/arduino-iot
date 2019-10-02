#include <SPI.h>
#include <MFRC522.h>

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define RST_PIN         D3          // Configurable, see typical pin layout above
#define SS_PIN          D8         // Configurable, see typical pin layout above

// LEDs config
#define BLUE_LED D0
#define WHITE_LED D1
#define BUZZER D2

#define PIN_ON HIGH
#define PIN_OFF LOW

// Golockserver config
String SERVER_HOST = "SERVERIP";
String SERVER_PORT = "SERVERPORT";

// WiFi config

// Replace WIFINAME with WiFi network name
// Replace WIFIPWD with WiFi network password
const char* ssid = "WIFINETWORK";
const char* password = "WIFIPASSWD";

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

void setup() {
  Serial.begin(115200);   // Initialize serial communications with the PC
  while (!Serial);  
  Serial.println("Begin");
  
  
  SPI.begin();      // Init SPI bus
  mfrc522.PCD_Init();   // Init MFRC522
  mfrc522.PCD_DumpVersionToSerial();  // Show details of PCD - MFRC522 Card Reader details
  Serial.println(F("Scan a card to see UID and data blocks..."));

  Serial.println("Configuring LEDs and the buzzer...");
  pinMode(BLUE_LED, OUTPUT);
  pinMode(WHITE_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  digitalWrite(BLUE_LED, PIN_ON);
  digitalWrite(WHITE_LED, PIN_OFF);
  digitalWrite(BUZZER, PIN_OFF);

  WiFi.begin(ssid, password);
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
  //Serial.println(WiFi.localIP());
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

  // Convert card uid in hexadecimal to a string to use it in the validation
  String cardUID = "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     cardUID.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : ""));
     cardUID.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  cardUID.toUpperCase();

//  String validationURL = "http://" + SERVER_HOST + ":" + SERVER_PORT + "/validate/" + cardUID;
  String validationURL = "http://" + SERVER_HOST + ":" + SERVER_PORT + "/validate/" + cardUID;

  
  // HTTP Client configuration
  HTTPClient http;
  http.begin(validationURL);
  // make request
  int httpCode = http.GET();
  // end request
  http.end();
  
  // Signals with LEDs
  digitalWrite(BLUE_LED, PIN_OFF);
  digitalWrite(WHITE_LED, PIN_ON);
  digitalWrite(BUZZER, PIN_ON);
  delay(750);
  digitalWrite(BUZZER, PIN_OFF);
  digitalWrite(WHITE_LED, PIN_OFF);
  digitalWrite(BLUE_LED, PIN_ON);
}
