#include <Arduino.h>
#include <DHTSensor.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <post.h>
#include <config.h>

#define exit() while (true) delay(100)

DHT dht1(13, SENSOR1_TYPE);
DHT dht2(12, SENSOR2_TYPE);

void setup() {
  Serial.begin(9600);

  Serial.printf("Connecting to WIFI with SSID %s.", WIFI_SSID);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    if (WiFi.status() == WL_CONNECT_FAILED) {
      Serial.println(F("\nFailed connect to WiFi!"));
      exit();
    } else {
      Serial.print('.');
    }
    delay(1000);
  }
  Serial.print("\nConnected with IP: ");
  Serial.println(WiFi.localIP());


  Serial.println(F("Checking Sensor connection..."));
  dht1.begin();
  dht2.begin();

  if (!checkDHTData(getDHTData(dht1))) {
    Serial.println(F("Failed to read from DHT1 sensor!"));
    exit();
  }

  if (!checkDHTData(getDHTData(dht2))) {
    Serial.println(F("Failed to read from DHT2 sensor!"));
    exit();
  }
  delay(1000);

}


void logSensorData(DHTSensorData sd, char* name, char* uuid) {
  String data = "{\"temp\":";
  data += sd.temparature;
  data += ",\"humidity\":";
  data += sd.humidity;
  data += ",\"name\":\"";
  data += name;
  data += "\",\"uuid\":\"";
  data += uuid;
  data += "\"}";
  Serial.println(data);
  Serial.println(data.length());
  WiFiClient client;
  if (client.connect(BACKEND_HOST, BACKEND_PORT)) {
    Serial.println("Connected.");
    char* req = genPostRequest(BACKEND_HOST , "/mcu/logdata", data.c_str(), data.length());
    int c = 0;
    do {
      Serial.write(req[c]);
      client.print(req[c]);
      c++;
    } while (req[c] != 0);
    delete[] req;
    client.stop();
    Serial.println();
  } else {
    Serial.println("Failed to connect.");
  }
}

void loop() {
  logSensorData(getDHTData(dht1), SENSOR1_NAME, SENSOR1_UUID);
  logSensorData(getDHTData(dht2), SENSOR2_NAME, SENSOR2_UUID);
  delay(6000);
}