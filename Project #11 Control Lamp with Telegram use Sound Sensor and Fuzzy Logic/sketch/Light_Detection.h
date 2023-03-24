// If using ESP32
#ifdef ESP32
#define lightDetect_Pin 5   // Light Sensor Output from pin GPIO 5
// If using ESP8266
#else
#define lightDetect_Pin D5  // Light Sensor Output from pin D5
#endif

int lightDetect;

void lightDetection() {
  lightDetect = digitalRead(lightDetect_Pin);
}