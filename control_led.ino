#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

#define LED_PIN D4

// Insert your network credentials
#define WIFI_SSID "JADHAV"
#define WIFI_PASSWORD "Adiasha@1004"

// Insert Firebase project API Key
#define API_KEY "AIzaSyDzl98R1cZ3fqsdWgIHu5bmBwv7P0pFzI8"

// Insert RTDB URL
#define DATABASE_URL "https://iot-project-fea41-default-rtdb.firebaseio.com/"

// Define Firebase Data object
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
int count = 0;
bool signupOK = false;

void setup(){
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  /* Assign the API key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  if (Firebase.ready() && signupOK) {
    // Check for incoming data from the database
    if (Firebase.RTDB.getInt(&fbdo, "control/onOff")) {
      if (fbdo.intData() == 0) {
        // Turn on the LED
        digitalWrite(LED_PIN, 0);
      } else {
        // Turn off the LED
        digitalWrite(LED_PIN, 1);
      }
    }
  }
}
