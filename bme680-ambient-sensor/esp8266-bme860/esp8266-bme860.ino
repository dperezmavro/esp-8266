#include "Adafruit_BME680.h"
#include <Adafruit_Sensor.h>
#include <ArduinoJson.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <Wire.h>

#define SEALEVELPRESSURE_HPA (1013.25)

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
  Serial.println(F("BME680 test"));

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

  char output[256];

  serializeJson(doc, output, sizeof(output));
  Serial.println(output);
  delay(2000);
}
