/*-------------------------------------------*/
// Including library
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// Initialize Serial and timer from Blynk
#define BLYNK_PRINT Serial
BlynkTimer timer;
/*-------------------------------------------*/

/*-------------------------------------------*/
// Initialize Blynk Auth Token, SSID, and Password
char auth[] = "Your_Blynk_Auth_Token";
char ssid[] = "Your_Credential_Ssid";
char pass[] = "Your_Credential_Password";
/*-------------------------------------------*/

const int pirPin = 14;   // Initialize PirPin from GPIO 14 or D5 pin
const int doorPin = 12;  // Initialize doorPin from GPIO 12 or D6 pin

/*-------------------------------------------*/
// Set all of state and function for know Door State
bool changeState = false;
bool state;
String doorState;

unsigned long previousMillis = 0;
const long interval = 1500;

ICACHE_RAM_ATTR void changeDoorStatus() {
  Serial.print("\n");
  Serial.println("Kondisi Berubah");
  changeState = true;
}
/*-------------------------------------------*/

void setup() {
  // Initialize baudrate
  Serial.begin(9600);

  // Start Blynk with auth, ssid, pass
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);

  // Set doorPin as INPUT_PULLUP
  pinMode(doorPin, INPUT_PULLUP);
  state = digitalRead(doorPin);
  attachInterrupt(digitalPinToInterrupt(doorPin), changeDoorStatus, CHANGE);

  pinMode(pirPin, INPUT);

  timer.setInterval(1000L, checkMotionState);
  timer.setInterval(1000L, checkDoorState);
}

void loop() {
  Blynk.run();
  timer.run();
}

void checkDoorState() {
  if (changeState) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      state = !state;
      if (state) {
        doorState = "Pintu Terbuka";
      } else {
        doorState = "Pintu Tertutup";
      }
      changeState = false;
      Serial.print(state);
      Serial.print(" : ");
      Serial.println(doorState);
      Serial.print("\n");

      // Update to Blynk
      Blynk.virtualWrite(V4, doorState);
    }
  }
}

void checkMotionState() {
  bool motionState = digitalRead(pirPin);
  String message;
  int statusLED = 0;
  Serial.print(motionState);
  Serial.print(" : ");
  if (motionState) {
    Serial.println("Gerak Terdeteksi");
    message = "Gerak Terdeteksi";
    statusLED = 1;
    Blynk.logEvent("pirmotion", "WARNING! Terdeteksi Gerakan!");
  } else {
    Serial.println("Tidak Terdeteksi Gerak");
    message = "Tidak Terdeteksi Gerak";
    statusLED = 0;
  }
  Blynk.virtualWrite(V0, message);
  Blynk.virtualWrite(V8, statusLED);
}