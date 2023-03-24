// If using ESP32
#ifdef ESP32
#include <WiFi.h>  // Include library Wifi for ESP32
#define relay 15   // Inizialize pin GPIO 15 for relay
// If using ESP8266
#else
#include <ESP8266WiFi.h>  // Include library Wifi for ESP8266
#define relay D1          // Inizialize pin D1 for relay
#endif

#include <WiFiClientSecure.h>      // Library Wifi Client Secure
#include <UniversalTelegramBot.h>  // Library Universal Telegram Bot Menggunakan Versi 1.3.0
#include <ArduinoJson.h>           // Library Arduino Json Menggunakan Versi 6.21.0


/************************************************************************************************/
#define SSID_Name "Your_Credential_Ssid"        // Inizialize name of SSID
#define PASSWORD "Your_Credential_Password"     // Inizialize Password
#define BOTtoken "Token_Http_API_Bot"           // Inizialize Token Bot Telegram from BotFather
#define CHAT_ID "Chat_Id"                       // Inizialize User Chat ID from IDBOT
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

#define hidup LOW
#define mati HIGH

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

  // Set relay as OUTPUT and first condition is LOW / OFF
  pinMode(relay, OUTPUT);
  digitalWrite(relay, mati);

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
  bot.sendMessage(CHAT_ID, "Halo, \nModule Sudah Siap Di Gunakan\nTekan : /start", "");
}

void loop() {
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
  }
}

// Function to control message from Telegram
void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  // If have message from telegram, the action is send back message to client
  for (int i = 0; i < numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID) {
      bot.sendMessage(chat_id, "Pengguna Tidak Terdaftar!", "");  // If client from chat_id not same with chat id above
      continue;
    }

    // Create variable text to get message data from client
    String text = bot.messages[i].text;
    Serial.println(text);
    String from_name = bot.messages[i].from_name;

    if (text == F("/relayON")) {  // If client send /relayON
      digitalWrite(relay, hidup);
      bot.sendChatAction(chat_id, "typing");
      delay(500);
      bot.sendMessage(chat_id, "Lampu sudah HIDUP / ON");  // Notification of condition
    } else if (text == F("/relayOFF")) {                   // If client send /relayOFF
      digitalWrite(relay, mati);
      bot.sendChatAction(chat_id, "typing");
      delay(500);
      bot.sendMessage(chat_id, "Lampu sudah MATI / OFF");  // Notification of condition
    } else if (text == F("/cekStatusRelay")) {             // If client send /cekStatusRelay
      bot.sendChatAction(chat_id, "typing");
      delay(500);
      if (digitalRead(relay) == hidup) {
        bot.sendMessage(chat_id, "Lampu Sedang Menyala / ON", "");  // Notification if condition Relay => LOW
      } else {
        bot.sendMessage(chat_id, "Lampu Dalam Kondisi Mati / OFF", "");  // Notification if condition Relay => HIGH
      }
    } else {
      // If client send /start
      if (text == F("/start")) {
        String welcome = "Hi " + from_name + " Tech ,\n";
        welcome += "Silakan Tekan Ini =>\n";
        welcome += "/relayON\n";
        welcome += "/relayOFF\n";
        welcome += "/cekStatusRelay\n";
        bot.sendMessage(chat_id, welcome);
      }
    }
  }
}