/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/telegram-esp8266-nodemcu-motion-detection-arduino/
  
  Project created using Brian Lough's Universal Telegram Bot Library: https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot
*/

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

// Replace with your network credentials
const char* ssid = "YOURSSID";
const char* password = "YOURPASS";

// Initialize Telegram BOT
#define BOTtoken "YOURTOKENTELEGRAM"  // your Bot Token (Get from Botfather)
                  

// Use @myidbot to find out the chat ID of an individual or a group
// Also note that you need to click "start" on a bot before it can
// message you
#define CHAT_ID "YOURCHATIDTELEGRAM"

X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

// Pin donde se encuentra conectado el Rele
#define PIN_RELE 12
// Pin donde se encuentra conectado el sensor PIR
#define PIN_PIR 14
// Pin donde se encuentra el led interno de nodemcu
#define PIN_LED 16

void estabilizar(){
  Serial.print("Estabilizar sensor");
  delay(60000);
}

void setup() {
  Serial.begin(9600);
  configTime(0, 0, "pool.ntp.org");      // get UTC time via NTP
  client.setTrustAnchors(&cert); // Add root certificate for api.telegram.org

  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_RELE, OUTPUT);
  pinMode(PIN_PIR, INPUT_PULLUP);
  Serial.begin(9600);

  // Attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Mirando por gatos");
  bot.sendMessage(CHAT_ID, "Mirando por gatos", "");
  digitalWrite(PIN_LED,LOW);

}

void loop() {
   // Si detectamos movimiento
   if (digitalRead(PIN_PIR) == 1) {
      Serial.println("Detectamos movimiento, valor sensor PIR: " + String(digitalRead(PIN_PIR)));
      Serial.println("Activamos el rele y definimos que se ha producido un movimiento");
      Serial.println("Gato pasando");
      Serial.println("Activando ultrasonido");
      for(int i = 0; i<5;i++){
        digitalWrite(PIN_RELE,HIGH);
        delay(500);
        digitalWrite(PIN_RELE,LOW);
        delay(200);
      }
      Serial.println("Desactivando ultrasonido");
      digitalWrite(PIN_RELE,LOW);
      Serial.println("Enviando aviso a mi master");
      digitalWrite(PIN_LED,HIGH);
      bot.sendMessage(CHAT_ID, "Ha pasado un gato!!", "");
      digitalWrite(PIN_LED,LOW);
      Serial.println("Aviso enviado");
    }
  delay(500);
}
