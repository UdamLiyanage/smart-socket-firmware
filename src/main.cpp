#include <Arduino.h>
#if defined(ESP8266)
#include <ESP8266WiFi.h>
#elif defined(ESP32)
#include <WiFi.h>
#include <WebServer.h>
#else
#error Invalid platform
#endif
#include <DNSServer.h>
#include <WiFiManager.h>
#include <Ticker.h>

/**
 * Constants.
 * URN = Unique Resource Number. A unique string the platform uses to differentiate between resources. 
 * */
#define SERIAL_NUMBER '0000000001'
#define URN 'fc1d82c0-c4cd-42fe-b8f5-da0b44f86a73'
#define CONFIG_LED 14

Ticker ticker;

void tick() {
    int state = digitalRead(CONFIG_LED);
    digitalWrite(CONFIG_LED, !state);
}

void configModeCallback (WiFiManager *wifiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  Serial.println(wifiManager->getConfigPortalSSID());
  ticker.attach(0.2, tick);
}

void setup() {
  Serial.begin(115200);
  pinMode(CONFIG_LED, OUTPUT);
  ticker.attach(0.6, tick);
  WiFiManager wifiManager;
  wifiManager.setAPCallback(configModeCallback);
  /** 
   * 
   * Disconnecting and Deleting WiFi for Testing Purposes. Remove in production
   * 
   * */
  wifiManager.disconnect();
  wifiManager.erase();
  //End of Test Code Snippet

  if (!wifiManager.autoConnect("SmartSocketAP")) {
    Serial.println("failed to connect and hit timeout");
    delay(1000);
  }
  Serial.println("Connected to Network!");
  ticker.detach();
  digitalWrite(CONFIG_LED, HIGH);
}

void loop() {
  digitalWrite(14, HIGH);
  delay(100);
  digitalWrite(14,LOW);
  delay(100);
}