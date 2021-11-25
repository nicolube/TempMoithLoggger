#include <Arduino.h>
#include <DHTSensor.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <WiFiClient.h>
#include <ArduinoOTA.h>
#include <post.h>
#include <config.h>

#define exit() delay(10000); ESP.restart() 

DHT dht1(13, SENSOR1_TYPE);
DHT dht2(12, SENSOR2_TYPE);

void setup() {
  Serial.begin(9600);

  Serial.printf("\n\nConnecting to WIFI with SSID %s.", WIFI_SSID);
  WiFi.mode(WIFI_STA);
  WiFi.hostname(WIFI_HOSTNAME);
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
  
  Serial.println("Setup OTA on port 8266");

  ArduinoOTA.setPort(8266);

  // Hostname defaults to esp3232-[MAC]
  ArduinoOTA.setHostname(WIFI_HOSTNAME);

  // No authentication by default
  ArduinoOTA.setPassword(OTA_PASS);
  
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_FS
      type = "filesystem";
    }

    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });

  ArduinoOTA.begin();

  Serial.println("Starting Program...\n\n");
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
  WiFiClient client;
  if (client.connect(BACKEND_HOST, BACKEND_PORT)) {
    Serial.println("Connected.");
    char* req = genPostRequest(BACKEND_HOST , "/mcu/logdata", data.c_str(), data.length());
    int c = 0;
    do {
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

unsigned long nTime = 0;

void loop() {
  ArduinoOTA.handle();
  if (millis() > nTime) {
    nTime = millis() + 600000;
    Serial.println("Logging data!");
    logSensorData(getDHTData(dht1), SENSOR1_NAME, SENSOR1_UUID);
    logSensorData(getDHTData(dht2), SENSOR2_NAME, SENSOR2_UUID);
  }
}