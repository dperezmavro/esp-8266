#ifndef WIFI_SETTINGS_H
#define WIFI_SETTINGS_H
#include <ESP8266WiFi.h>
#include "./secrets.h"

const char* wifi_ssid = WIFI_SSID;
const char* wifi_pass = WIFI_PASS;
#endif  // WIFI_SETTINGS_H

// BME680 settings
#ifndef SENSOR_SETTINGS_H
#define SENSOR_SETTINGS_H

#include <Adafruit_BME680.h>
#include <Adafruit_Sensor.h>
#include <ArduinoJson.h>
#include <Wire.h>


#define SEALEVELPRESSURE_HPA (1013.25)
#define SAMPLE_INTERVAL_MS 5000
#define OUTPUT_BUFFER_SIZE 256

Adafruit_BME680 bme(&Wire);  // I2C

float temperature;
float humidity;
float pressure;
float gasResistance;
float altitude;
float dewPont;
JsonDocument doc;
#endif  // SENSOR_SETTINGS_H

// MQTT topic to publish sensor data

#ifndef MQTT_SETTINGS_H
#define MQTT_SETTINGS_H
#include <AwsIotWiFiClient.h>
#define AWS_IOT_PUBLISH_TOPIC "sensors/pub"

AwsIotWiFiClient awsIotWiFiClient;

BearSSL::X509List trustAnchorCertificate(cacert);
BearSSL::X509List clientCertificate(client_cert);
BearSSL::PrivateKey clientPrivateKey(privkey);

void connectAWS() {
  Serial.println("[*] Connecting to AWS");
  // Set up AWS IoT Wi-Fi Client:
  awsIotWiFiClient
    // Enable debug output:
    .setDebugOutput(true)
    // Certificates to establish secure communication (defined above):
    .setCertificates(&trustAnchorCertificate, &clientCertificate, &clientPrivateKey)
    // Device Data Endpoint from IoT Core -> Settings:
    .setEndpoint(MQTT_HOST)
    // MQTT client ID aka thing name:
    .setSubscribeTopicFilter("subscribeTopicFilter")
    .setClientId(THING_NAME)
    // Connect to the AWS IoT service:
    .connect();

  Serial.println("AWS IoT Connected!");
}
#endif  // MQTT_SETTINGS_H


void setup_sensors() {
  Serial.println(F("[*] Starting BME sensor setup"));

  if (!bme.begin()) {
    Serial.println("Could not find a valid BME680 sensor, check wiring!");
    while (1)
      ;
  }
  // Set up oversampling and filter initialization
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150);  // 320*C for 150 ms
  Serial.println(F("[+] BME sensor setup successful!"));
}

void setup_wifi() {
  Serial.println(F("[*] Starting WiFi setup"));
  WiFi.begin(wifi_ssid, wifi_pass);

  Serial.println("[*] Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.printf("  last status: %d.\n", WiFi.status());
  }
  Serial.printf("[+] Connected, IP address: %s\n", WiFi.localIP().toString().c_str());
  Serial.println(F("[+] WiFi setup successful!"));
}

void setup() {
  Serial.begin(9600);
  while (!Serial)
    ;
  Serial.println();
  Serial.println(F("ESP8266 + BME680 starting up"));

  setup_wifi();
  setup_sensors();
  connectAWS();
}

void loop() {
  if (!bme.performReading()) {
    Serial.println("Failed to perform reading :(");
    return;
  }

  doc["sensor"] = "bme680";
  doc["location"] = "living_room";
  doc["altitude (m)"] = bme.readAltitude(SEALEVELPRESSURE_HPA);
  doc["gas (KOhms)"] = bme.gas_resistance / 1000.0;
  doc["humidity (%)"] = bme.humidity;
  doc["pressure (hPa)"] = bme.pressure;
  doc["temperature (*C))"] = bme.temperature;

  char jsonBuffer[OUTPUT_BUFFER_SIZE];

  serializeJson(doc, jsonBuffer, sizeof(jsonBuffer));
  Serial.println(jsonBuffer);

  awsIotWiFiClient.publishMessage(AWS_IOT_PUBLISH_TOPIC, jsonBuffer);
  awsIotWiFiClient.loop();
  delay(SAMPLE_INTERVAL_MS);
}
