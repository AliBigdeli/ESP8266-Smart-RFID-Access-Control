#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <SPI.h>
#include <MFRC522.h>

String cardReaderUID = "abc123";

//Variables
int i = 0;
int statusCode;
String content;
String eeprom_ssid;
String eeprom_pass;
String req_ssid;
String req_pass;

// wifi and security variables
const char * access_point_name = "ESP8266 AP";
const char * mdns_wifi_name = "esp8266";
const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 4, 1);


//Creating server instance for web
DNSServer dnsServer;
ESP8266WebServer webServer(80);

// Configurable, see typical pin layout above
#define RST_PIN         D3
// Configurable, see typical pin layout above
#define SS_PIN          D8
// buzzer pin
#define BUZZER_PIN      D2


// Create MFRC522 instance
MFRC522 mfrc522(SS_PIN, RST_PIN);

// Server Address
String serverAddress = "http://192.168.1.144/dashboard/api/card-reader/gateway/";

// global variables
byte letter;
int httpResponseCode;
String res_message;
String httpResponsePayload;
bool place_action = false;


WiFiClient client;
HTTPClient http;

void setup() {
  Serial.begin(115200);
  Serial.println();

  Serial.println("Disconnecting previously connected WiFi");
  WiFi.disconnect();

  //Initializing EEPROM
  EEPROM.begin(512);

  // feeding the watchdog
  delay(10);

  // clearing the serial monitor
  Serial.println();
  Serial.println();
  Serial.println("Startup");

  Serial.println("setting up rfid reader");
  SPI.begin();      // Init SPI bus
  mfrc522.PCD_Init();   // Init MFRC522
  delay(4);       // Optional delay. Some board do need more time after init to be ready, see Readme
  mfrc522.PCD_DumpVersionToSerial();  // Show details of PCD - MFRC522 Card Reader details

  Serial.println("setup relays");
  pinMode(D1,OUTPUT);
  pinMode(BUZZER_PIN,OUTPUT);
  digitalWrite(D1,HIGH);
  // reading eeprom and geting last wifi credentials
  read_eeprom();

  //  try to connect to wifi for a period of time if it fails will open hotspot
  if (testWifi())
  {
    Serial.println("Succesfully Connected!!!");
    return;
  }
  else
  {
    Serial.println("Turning the HotSpot On");
    createWebServer();
  }
}

void loop() {
  if ((WiFi.status() == WL_CONNECTED))
  {
    readCardUID();
    changeGateState();
  }
  else
  {
    dnsServer.processNextRequest();
    webServer.handleClient();
  }
}
