const int sampleWindow = 50;  // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;          // Value from Sound Sensor
int db;                       // Value in dB
float p, t, k, pintu, tepukTangan, kendaraan;

// Function for detect sound
void soundDetect() {
  unsigned long startMillis = millis();  // Start of sample window
  float peakToPeak = 0;                  // Peak-to-peak level

  unsigned int signalMax = 0;     // Minimum value
  unsigned int signalMin = 1024;  // Maximum value

  // collect data for 50 mS
  while (millis() - startMillis < sampleWindow) {

// If using ESP32
#ifdef ESP32
    sample = analogRead(36);                // Get reading from pin GPIO 36
  //  If using ESP8266
#else
    sample = analogRead(0);                 // Get reading from pin A0
#endif

    if (sample < 1024)                      // Toss out spurious readings
    {
      if (sample > signalMax) {
        signalMax = sample;                 // Save just the max levels
      } else if (sample < signalMin) {
        signalMin = sample;                 // Save just the min levels
      }
    }
  }
  peakToPeak = signalMax - signalMin;       // Max - min = peak-peak amplitude
  // Serial.println(peakToPeak);
  db = map(peakToPeak, 20, 900, 49.5, 90);  // Calibrate for deciBels
}

// Function to set value with Fuzzy Logic Formula
void fuzzyLogic() {
  // Pintu = 50 - dB / 50 - 40
  p = 50 - db;
  pintu = p / 10;

  // Tepuk Tangan = dB - 40 / 60 - 40
  t = db - 40;
  tepukTangan = t / 20;

  // Kendaraan = dB - 60 / 70 - 60
  k = db - 60;
  kendaraan = k / 10;

  // Displaying value to Serial Monitor
  Serial.print("\t\t\t");
  Serial.print(pintu);
  Serial.print("\t");
  Serial.print(tepukTangan);
  Serial.print("\t");
  Serial.println(kendaraan);
}