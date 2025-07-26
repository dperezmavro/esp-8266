// #include "./aws_iot_client.h"
#include "./influx_db.h"

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

#define DEVICE "bme680"

InfluxDB influx_db(DEVICE);


void setup_sensors() {
  Serial.println(F("[*] Starting BME sensor setup"));

  while (!bme.begin()) {
    Serial.println("Could not find a valid BME680 sensor, check wiring!");
    delay(1500);
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
    delay(1500);
    Serial.printf("  last status: %d.\n", WiFi.status());
  }
  Serial.printf("[+] Connected, IP address: %s\n", WiFi.localIP().toString().c_str());
  Serial.println(F("[+] WiFi setup successful!"));
}

void setup_ntp() {
  Serial.println(F("[*] Starting NTP setup"));
  timeSync(TZ_INFO, "pool.ntp.org", "time.nis.gov");
  Serial.println(F("[+] NTP setup successful!"));
}

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    delay(1000);
  }

  Serial.println();
  Serial.println(F("ESP8266 + BME680 starting up"));
  
  
  setup_wifi();
  setup_ntp();
  setup_sensors();
  
  if (!influx_db.setup()) {
    Serial.println(F("InfluxDB setup failed"));
    return;
  }

  Serial.println(F("[+] Setup completed successfully!"));
}

void loop() {
  
  if (!bme.performReading()) {
    Serial.println("Failed to perform reading :(");
    delay(1000);
    return;
  }


  std::map<std::string, float> values;
  values["temperature (C)"] = bme.temperature;
  values["humidity (%)"] = bme.humidity;
  values["pressure (hPa)"] = bme.pressure;
  values["gas (KOhms)"] = bme.gas_resistance / 1000.0;
  values["altitude (m)"] = bme.readAltitude(SEALEVELPRESSURE_HPA);

  // values["location"] = "living_room";
  // values["sensor_device"] = DEVICE;
  

  influx_db.write_point(values);
  Serial.println("Waiting 1 second");

  delay(SAMPLE_INTERVAL_MS);
}
