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

void setup() {
    Serial.begin(115200);
    pinMode(12, OUTPUT);
    WiFiManager wifiManager;
    wifiManager.autoConnect("AutoConnectAP");
    Serial.println("connected...yeey :)");
}

void loop() {
  digitalWrite(12, HIGH);
  delay(100);
  digitalWrite(12,LOW);
  delay(100);
}