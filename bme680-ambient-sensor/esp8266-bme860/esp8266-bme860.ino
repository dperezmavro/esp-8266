#include <Adafruit_BME680.h>
#include <ESP8266WiFi.h>
#include <Adafruit_Sensor.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include "./secrets.h"

#define SEALEVELPRESSURE_HPA (1013.25)
#define SAMPLE_INTERVAL_MS 5000
#define OUTPUT_BUFFER_SIZE 256

const char* wifi_ssid = WIFI_SSID;
const char* wifi_pass = WIFI_PASS;

Adafruit_BME680 bme(&Wire);  // I2C

float temperature;
float humidity;
float pressure;
float gasResistance;
float altitude;
float dewPont;


void setup() {
  Serial.begin(9600);
  while (!Serial)
    ;
  Serial.println();
  Serial.println(F("ESP8266 + BME680 starting up"));

  if (!bme.begin()) {
    Serial.println("Could not find a valid BME680 sensor, check wiring!");
    while (1)
      ;
  }

  Serial.println(F("[*] Starting WiFi setup"));
  WiFi.begin(wifi_ssid, wifi_pass);

  Serial.println("[*] Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.printf("  last status: %d.\n", WiFi.status());
  }
  Serial.printf("[+] Connected, IP address: %s\n", WiFi.localIP().toString().c_str());
  Serial.println(F("[+] WiFi setup successful!"));


  Serial.println(F("[*] Starting BME sensor setup"));
  // Set up oversampling and filter initialization
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150);  // 320*C for 150 ms
  Serial.println(F("[+] BME sensor setup successful!"));
}


JsonDocument doc;


void loop() {
  if (!bme.performReading()) {
    Serial.println("Failed to perform reading :(");
    return;
  }

  doc["sensor"] = "bme680";
  doc["altitude (m)"] = bme.readAltitude(SEALEVELPRESSURE_HPA);
  doc["gas (KOhms)"] = bme.gas_resistance / 1000.0;
  doc["humidity (%)"] = bme.humidity;
  doc["pressure (hPa)"] = bme.pressure;
  doc["temperature (*C))"] = bme.temperature;

  char output[OUTPUT_BUFFER_SIZE];

  serializeJson(doc, output, sizeof(output));
  Serial.println(output);
  delay(SAMPLE_INTERVAL_MS);
}
