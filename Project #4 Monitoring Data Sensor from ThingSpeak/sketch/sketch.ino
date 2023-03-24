// Include all library
#include <WiFi.h>
#include "ThingSpeak.h"

const char* ssid = "TP-Link_B3CA";  // Write your SSID
const char* password = "59917557";  // Write your WIFI password

WiFiClient client;

unsigned long Channel_ID = 1;  //replace with your Channel ID
const char* API_key = "9QYN5LQ42M8VQJG8";

unsigned long last_time = 0;
unsigned long Delay = 10000;

const int trigPin = 15;
const int echoPin = 2;

//define sound speed in cm/uS
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

long duration;
float distanceCm;

void setup() {
  Serial.begin(115200);

  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input

  // Set WIFI Station mode
  WiFi.mode(WIFI_STA);

  ThingSpeak.begin(client);
}

void loop() {
  if ((millis() - last_time) > Delay) {
    if (WiFi.status() != WL_CONNECTED) {
      Serial.print("Connecting...");
      while (WiFi.status() != WL_CONNECTED) {
        WiFi.begin(ssid, password);
        delay(5000);
      }
      Serial.println("\nConnected.");
    }

    detection();

    int Data = ThingSpeak.writeField(Channel_ID, 1, distanceCm, API_key);

    if (Data == 200) {
      Serial.println("Channel updated successfully!");
    } else {
      Serial.println("Problem updating channel. HTTP error code " + String(Data));
    }
    last_time = millis();
  }
}

void detection(){
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculate the distance
  distanceCm = duration * SOUND_SPEED/2;
  
  // Prints the distance in the Serial Monitor
  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);
}