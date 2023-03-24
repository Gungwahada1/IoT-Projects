/*==========================================================================================
  {
    "name":"Project #11 --> Control Relay via Telegram",
    "description":"Control relay using Telegram Bot, detection sound and light using sensor, and control ON/OFF relay via Bot.",
    "keywords":"Communication,Telegram,Arduino,ESP32,ESP8266,FuzzyLogic,Tsukamoto",
    "authors":
    {
      "name": "Gungwah <gungwahada1@gmail.com>",
      "telegram": "@agungsemaraa"
      "instagram": "@agungsemaraa"
    },
    "repository":
    {
      "type": "git",
      "url": "",
    },
    "version":
    {
      "UniversalTelegramBot": "1.3.0",
      "ArduinoJson": "6.20.1",
      "esp32": "2.0.7",
      "esp8266": "3.1.1",
    }
  }
==========================================================================================*/

#include "Sound_Detection.h"  // Include file on same folder
#include "Light_Detection.h"  // Include file on same folder

// If using ESP32
#ifdef ESP32
#include <WiFi.h>  // Include library Wifi for ESP32
#define relay 2    // Inizialize pin GPIO 2 for relay
// If using ESP8266
#else
#include <ESP8266WiFi.h>  // Include library Wifi for ESP8266
#define relay D1          // Inizialize pin D1 for relay
#define supply D6         // Inizialize pin D5 for supply
#endif

#include <WiFiClientSecure.h>      // Library Wifi Client Secure
#include <UniversalTelegramBot.h>  // Library Universal Telegram Bot Menggunakan Versi 1.3.0
#include <ArduinoJson.h>           // Library Arduino Json Menggunakan Versi 6.15.2

/************************************************************************************************/
// #define SSID_Name "Gungwah ada berapa?"
// #define PASSWORD "Gungwahada1"
#define SSID_Name "TP-Link_B3CA"                                   // Inizialize name of SSID
#define PASSWORD "59917557"                                        // Inizialize Password
#define BOTtoken "6134335313:AAHJ7EUtDmC8YGwme7JtUfDWMkriXYc4SsY"  // Inizialize Token Bot Telegram from BotFather
#define CHAT_ID "1074113887"                                       // Inizialize User Chat ID from IDBOT
/************************************************************************************************/

// If using ESP8266
#ifdef ESP8266
X509List cert(TELEGRAM_CERTIFICATE_ROOT);  // SSL Telegram Certificate
#endif

/************************************************************************************************/
WiFiClientSecure client;                     // Create variable name for client
UniversalTelegramBot bot(BOTtoken, client);  // Create variable name for bot

const char* ssid = SSID_Name;     // Change SSID_Name as pointer ssid
const char* password = PASSWORD;  // Change PASSWORD as pointer password

int delayRequestBot = 1000;    // Delay for request with bot
unsigned long lastTimeBotRun;  // Inizialize variable lastTimeBotRun

unsigned long lightTimerExpires;
boolean lightTimerActive = false;
/************************************************************************************************/

#define ON HIGH
#define OFF LOW
#define hidup LOW
#define mati HIGH

// Light Detection (0 => Light & 1 => Dark)
int light = 0;
int dark = 1;

// Function to control message from Telegram
void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  // If have message from telegram, the action is send back message to client
  for (int i = 0; i < numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID) {
      bot.sendMessage(chat_id, "User unregistered!", "");  // If client from chat_id not same with chat id above
      continue;
    }

    // Create variable text to get message data from client
    String text = bot.messages[i].text;
    Serial.println(text);
    String from_name = bot.messages[i].from_name;

    if (text == F("/relayON")) {  // If client send /relayON
      digitalWrite(relay, hidup);
      bot.sendChatAction(chat_id, "typing");
      delay(1000);
      bot.sendMessage(chat_id, "Menyala");  // Notification of condition
    } else if (text == F("/relayOFF")) {    // If client send /relayOFF
      digitalWrite(relay, mati);
      bot.sendChatAction(chat_id, "typing");
      delay(1000);
      bot.sendMessage(chat_id, "Mati");         // Notification of condition
    } else if (text == F("/cekStatusRelay")) {  // If client send /cekStatusRelay
      if (digitalRead(relay) == hidup) {
        bot.sendMessage(chat_id, "Relay Sedang Menyala / ON", "");  // Notification if condition Relay => LOW
      } else {
        bot.sendMessage(chat_id, "Relay Dalam Kondisi Mati / OFF", "");  // Notification if condition Relay => HIGH
      }
    } else {
      // If client send /start
      if (text == F("/start")) {
        String welcome = "Hi " + from_name + ",\n";
        welcome += "Silakan Tekan Ini =>\n";
        welcome += "/tombol\n";
        welcome += "/relayON\n";
        welcome += "/relayOFF\n";
        bot.sendMessage(chat_id, welcome);
      }
      // If client send /tombol
      if (text == F("/tombol")) {
        String Kirim_Perintah = F("[[{ \"text\" : \"ON\", \"callback_data\" : \"/relayON\" },{ \"text\" : \"OFF\", \"callback_data\" : \"/relayOFF\" }],");
        Kirim_Perintah += F("[{ \"text\" : \"            Cek Status Relay             \", \"callback_data\" : \"/cekStatusRelay\" }]]");
        bot.sendMessageWithInlineKeyboard(chat_id, "\nKontrol Relay", "", Kirim_Perintah);
      }
    }
  }
}

void setup() {
  Serial.begin(115200);

// If using ESP8266
#ifdef ESP8266
  // Check the SSL Certificate
  Serial.println("Menggunakan Board ESP8266");
  configTime(0, 0, "pool.ntp.org");
  client.setTrustAnchors(&cert);
#endif

// If using ESP32
#ifdef ESP32
  // Check the SSL Certificate
  Serial.println("Menggunakan Board ESP32");
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
#endif

  // Set lightDetect_Pin as INPUT
  pinMode(lightDetect_Pin, INPUT);
  // Set relay as OUTPUT and first condition is LOW / OFF
  pinMode(relay, OUTPUT);
  digitalWrite(relay, hidup);
  // Set supply as INPUT and first condition is LOW 
  pinMode(supply, OUTPUT);
  digitalWrite(supply, LOW);

  // Set WIFI Station mode
  WiFi.mode(WIFI_STA);
  // Connecting to WIFI
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi -> ");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  // Displaying IP Address
  Serial.println("");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
  bot.sendMessage(CHAT_ID, "Halo, \nModule Sudah Siap Di Gunakan\nTekan : \n/start\n/tombol", "");
}

void loop() {
  // Running the void function for detect sound, light, and fuzzy logic
  soundDetect();
  lightDetection();
  fuzzyLogic();

  // Displaying dB and status Lighting
  Serial.print("Suara dalam dB : ");
  Serial.println(db);
  Serial.print("Status Cahaya : ");
  Serial.print(lightDetect);
  Serial.print("\t");
  if (lightDetect == 1) {
    Serial.println("Gelap");
  } else if (lightDetect == 0) {
    Serial.println("Ada Cahaya");
  }

  if (pintu > tepukTangan && pintu > kendaraan) {  // If value sound Pintu > Tepuk Tangan OR Pintu > Kendaraan
    Serial.println("\t\t\tSuara : Pintu");
    digitalWrite(relay, mati);
  } else if (db <= 30) {  // If value dB <= 30
    Serial.println("\t\t\tSuara : Pintu");
    digitalWrite(relay, mati);
  }
  if (tepukTangan > pintu && tepukTangan > kendaraan) {  // If value sound Tepuk Tangan > Pintu OR Tepuk Tangan > Kendaraan
    Serial.println("\t\t\tSuara : Tepuk Tangan");
    digitalWrite(relay, hidup);
    bot.sendMessage(CHAT_ID, "Suara Tepuk Tangan, Lampu Menyala", "");  // Send message if condition is true
    // state = true;
  }
  if (kendaraan > tepukTangan && kendaraan > pintu) {  // If value sound Kendaraan > Tepuk Tangan OR Kendaraan > Pintu
    Serial.println("\t\t\tSuara : Kendaraan");
    digitalWrite(relay, mati);
  } else if (db >= 80) {  // If value dB >= 80
    Serial.println("\t\t\tSuara : Kendaraan");
    digitalWrite(relay, mati);
  }

  // Run detect if new messages from Telegram or not
  if (millis() > lastTimeBotRun + delayRequestBot) {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    // If got response from Telegram
    while (numNewMessages) {
      Serial.println("Got Renponse!");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRun = millis();
  } else if (digitalRead(relay) == mati) {  // If relay OFF from Telegram, non-active relay
    digitalWrite(relay, mati);
    Serial.println("Lampu Dimatikan Paksa Melalui Telegram");
  } else if (digitalRead(relay) == hidup) {  // If relay ON from Telegram, sensor work normal detection again
    switch (lightDetect) {
      case 0:
        digitalWrite(relay, hidup);
        Serial.println("Lampu Menyala Normal");
        break;
      case 1:
        digitalWrite(relay, hidup);
        Serial.println("Lampu Menyala Normal");
        break;
    }
  }
}