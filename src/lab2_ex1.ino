#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const int redLED = 26;
const int greenLED = 27;
const int blueLED = 14;
const int yellowLED = 12;

const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* readAPI = "http://api.thingspeak.com/channels/3109942/feeds.json?results=1";

void setup() {
  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(blueLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  digitalWrite(redLED, LOW);
  digitalWrite(greenLED, LOW);
  digitalWrite(blueLED, LOW);
  digitalWrite(yellowLED, LOW);

  Serial.begin(115200);
  Serial.println("Ziyo, Lab 2");

  WiFi.begin(ssid, password);
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected! IP: " + WiFi.localIP().toString());
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Reconnecting...");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) delay(500);
    Serial.println("Reconnected! IP: " + WiFi.localIP().toString());
  }

  HTTPClient http;
  http.begin(readAPI);
  int code = http.GET();

  if (code == 200) {
    String json = http.getString();
    Serial.println(json);

    StaticJsonDocument<1024> doc;
    if (deserializeJson(doc, json) == DeserializationError::Ok) {
      JsonObject f = doc["feeds"][0];
      digitalWrite(redLED,    f["field1"] == "HIGH" ? HIGH : LOW);
      digitalWrite(greenLED,  f["field2"] == "HIGH" ? HIGH : LOW);
      digitalWrite(blueLED,   f["field3"] == "HIGH" ? HIGH : LOW);
      digitalWrite(yellowLED, f["field4"] == "HIGH" ? HIGH : LOW);
    } else {
      Serial.println("JSON parse failed");
    }
  } else {
    Serial.println("HTTP error: " + String(code));
  }

  http.end();
  delay(5000);
}
