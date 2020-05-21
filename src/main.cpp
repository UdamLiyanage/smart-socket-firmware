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
#include <PubSubClient.h>
#include <ArduinoJson.h>

/**
 * Constants.
 * URN = Unique Resource Number. A unique string the platform uses to differentiate between resources. 
 * */
#define SERIAL_NUMBER '0000000001'
//#define URN 'fc1d82c0-c4cd-42fe-b8f5-da0b44f86a73'
#define CONFIG_LED 14

const String URN = "fc1d82c0-c4cd-42fe-b8f5-da0b44f86a73";
const String UPDATE_TOPIC = "/althinect/things/fc1d82c0-c4cd-42fe-b8f5-da0b44f86a73/update";
const String DELTA_TOPIC = "/althinect/things/" + URN + "/update/delta";

Ticker ticker;

WiFiClient espClient;
PubSubClient client(espClient);

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  if (String(topic) == "/althinect/things/fc1d82c0-c4cd-42fe-b8f5-da0b44f86a73/shadow/update/delta") {
    Serial.println("Delta Topic");
    const size_t capacity = JSON_OBJECT_SIZE(1) + 10;
    DynamicJsonDocument doc(capacity);

    deserializeJson(doc, payload, length);

    if (doc["state"] == "on") {
      Serial.println("ON");
      digitalWrite(12, HIGH);
      client.publish("/test", "State Changed to ON");
    } else {
      Serial.println("OFF");
      digitalWrite(12, LOW);
      client.publish("/test", "State Changed to OFF");
    }
  } 
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("arduinoClient")) {
      Serial.println("connected");
      client.publish("/althinect/things/fc1d82c0-c4cd-42fe-b8f5-da0b44f86a73/heartbeat", ("Connected"));
      client.subscribe("/althinect/things/fc1d82c0-c4cd-42fe-b8f5-da0b44f86a73/shadow/update/delta");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

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
  pinMode(12, OUTPUT);
  ticker.attach(0.6, tick);
  WiFiManager wifiManager;
  wifiManager.setAPCallback(configModeCallback);
  /** 
   * 
   * Disconnecting and Deleting WiFi for Testing Purposes. Remove in production
   * 
   * */
  //wifiManager.disconnect();
  //wifiManager.erase();
  //End of Test Code Snippet

  if (!wifiManager.autoConnect("SmartSocketAP")) {
    Serial.println("failed to connect and hit timeout");
    delay(1000);
  }
  Serial.println("Connected to Network!");
  ticker.detach();
  digitalWrite(CONFIG_LED, HIGH);

  client.setServer("broker.abydub.com", 1883);
  client.setCallback(callback);
  delay(1500);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}