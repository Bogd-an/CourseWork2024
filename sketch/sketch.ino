// ESP32 dev board
#include <WiFi.h>
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <HTTPClient.h>

//////////////////////////////////////////////////
const char* WIFI_SSID     = "???????";
const char* WIFI_PASSWORD = "???????";
const char* server_address   = "???????/sensor";
const char* key   = "qwerty1234";
//////////////////////////////////////////////////

// Sensor pins
const byte pin_sensor_pir  = 35;
const byte pin_sensor_lock = 23;

// Keypad settings
byte pin_rows[4]   = {13, 12, 14, 27};
byte pin_column[4] = {26, 25, 33, 32};
char keys[4][4] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
Keypad keypad = Keypad(makeKeymap(keys), pin_rows, pin_column, 4, 4);

// LCD settings
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Variables
String password = "";
bool pirState   = false;
bool lockState  = false;

bool pirStatePrev   = false;
bool lockStatePrev  = false;

void setup() {
  lcd.init();
  lcd.backlight();
  pinMode(pin_sensor_pir, INPUT);
  pinMode(pin_sensor_lock, INPUT_PULLUP);
  Serial.begin(115200);

  lcd.print("Try connect");
  lcd.setCursor(0, 1);
  lcd.print(WIFI_SSID);
  Serial.print("\nConnecting to WiFi SSID " + String(WIFI_SSID));
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.print("\nWiFi connected. IP address: " + String(WiFi.localIP()));
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WiFi connected");

  sendSensorData();
}


void loop() {
  // Check keypad input
  char key = keypad.getKey();
  if (key) handleKeyInput(key);

  // Check PIR, lock sensor
  pirState  =  digitalRead(pin_sensor_pir);
  lockState = !digitalRead(pin_sensor_lock);
  
  // Display status
  lcd.setCursor(0, 0);
  lcd.print("PIR=" +   String(pirState  ? "yes" : "no ") +
            " Lock=" + String(lockState ? "yes" : "no ") );

  lcd.setCursor(0, 1);
  lcd.print(WiFi.status() == WL_CONNECTED ? " Online" : "Offline");

  lcd.setCursor(7, 1);
  lcd.print("     ");

  lcd.setCursor(12, 1);
  for (byte i = 0; i < 4; i++)
    lcd.print(i < password.length() ? '*' : ' ');

  if(pirState != pirStatePrev){
    pirStatePrev = pirState;
    if(pirState) sendSensorData();
   }
   if(lockState != lockStatePrev){
    lockStatePrev = lockState;
    sendSensorData();
   }
}


void handleKeyInput(char key) {
  if (key == '*' && password.length() == 4) {
    sendSensorData();
    password = "";
  }
  else if (key == '#') password = "";
  else if (password.length() < 4) password += key;
  else password = "";
}

void sendSensorData() {
  String json = "{ \"pass\":\"" + password + "\","
                  "\"pir\":"    + String(pirState  ? "true" : "false") +
                  ",\"lock\":"  + String(lockState ? "true" : "false") +
                  ",\"key\":\"" + String(key) +"\""+
                "}";
  Serial.println("JSON: " + json);

  lcd.setCursor(7, 1);
  lcd.print("<send");
  
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(server_address);
    http.addHeader("Content-Type", "application/json");
     int httpResponseCode = http.POST(json);
    if (httpResponseCode > 0)
      Serial.print("HTTP Response code: ");
    else
      Serial.print("Error code: ");
    Serial.println(httpResponseCode);
    http.end();
  }
}