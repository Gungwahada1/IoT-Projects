/*-------------------------------------------*/
// Include library file
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// Enable output to Serial monitor and initialize timer from Blynk
#define BLYNK_PRINT Serial
BlynkTimer timer;
/*-------------------------------------------*/

/*-------------------------------------------*/
// Set your auth token from Blynk and your credentials
char auth[] = "Your_Blynk_Auth_Token";
char ssid[] = "Your_Credential_Ssid";
char pass[] = "Your_Credential_Password";
/*-------------------------------------------*/

// Initialize pin for LDR sensor, Magnetic Door Sensor, and relay
const int ldrPin = 14;  // D5
const int relay = 5;    // D1

// To control relay
#define hidup LOW
#define mati HIGH

void setup() {
  // Set serial communication baudrate
  Serial.begin(9600);

  // Set blynk to connect with credentials, blynk cloud, and port
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);

  // Set ldrPin to INPUT mode
  pinMode(ldrPin, INPUT);

  // Set mode to relay
  pinMode(relay, OUTPUT);
  digitalWrite(relay, mati);

  // Set timer for update data from Blynk
  timer.setInterval(1000L, checkLightState);
}

void loop() {
  // Run Blynk and timer
  Blynk.run();
  timer.run();
}

// Void to run and check light state
void checkLightState() {
  // check light state first
  bool lightState = digitalRead(ldrPin);
  String message;
  int statusLED = 0;
  Serial.print(lightState);  // Send to Serial Monitor
  Serial.print(" : ");
  if (lightState) {
    Serial.println("Kondisi Gelap!");
    message = "Kondisi Gelap, Lampu Menyala";
    statusLED = 1;
    digitalWrite(relay, hidup);
  } else {
    Serial.println("Kondisi Terang!");
    message = "Kondisi Terang, Lampu Dimatikan";
    statusLED = 0;
    digitalWrite(relay, mati);
  }
  Blynk.virtualWrite(V0, message);    // Send message data for datastreams V0 from Blynk and update
  Blynk.virtualWrite(V4, statusLED);  // Send statusLED data for datastreams V4 from Blynk and update
}