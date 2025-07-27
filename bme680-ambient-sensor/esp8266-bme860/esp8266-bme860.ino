#include "./influx_db.h"
#include "./sensor.h"
#include "./setup.h"

#define SAMPLE_INTERVAL_MS 10000
#define SETUP_RETRY_MS 2000
#define BAUD_RATE 9600

BME680 bme("studio");
InfluxDB influx_db(bme.get_device_name());

void setup() {
  Serial.begin(BAUD_RATE);
  while (!Serial) {
    delay(SETUP_RETRY_MS);
  }

  Serial.println();
  Serial.println(F("ESP8266 + BME680 starting up"));

  while (!bme.setup()) {
    Serial.println(F("[-] BME sensor setup failed"));
    delay(SETUP_RETRY_MS);
  }

  setup_wifi();
  setup_ntp();

  while (!influx_db.setup()) {
    Serial.println(F("[-] InfluxDB setup failed"));
    delay(SETUP_RETRY_MS);
  }

  Serial.println(F("[+] Setup completed successfully!"));
}

void loop() {
  influx_db.write_point(bme.read_tags(), bme.read_values());
  
  Serial.printf("Waiting %ds\n", SAMPLE_INTERVAL_MS/1000);
  delay(SAMPLE_INTERVAL_MS);
}