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

int blink(int blinkPin);

void setup() {
  Serial.begin(115200);
  pinMode(12, OUTPUT);
  WiFiManager wifiManager;
  /** 
   * 
   * Disconnecting and Deleting WiFi for Testing Purposes. Remove in production
   * 
   * */
  wifiManager.disconnect();
  wifiManager.erase();
  //End of Test Code Snippet
  
  wifiManager.autoConnect("AutoConnectAP");
  int k = 0;
  k = blink(12);
  if (k == 1) {
    Serial.println("Connected to Network");
  } else {
    Serial.println("Network Connection Failed");
  }
}

void loop() {
  digitalWrite(12, HIGH);
  delay(100);
  digitalWrite(12,LOW);
  delay(100);
}