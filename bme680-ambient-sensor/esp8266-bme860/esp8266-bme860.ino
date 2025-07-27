#include "./influx_db.h"
#include "./sensor.h"
#include "./setup.h"

BME680 bme("studio");
InfluxDB influx_db(bme.get_device_name());

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    delay(1000);
  }

  Serial.println();
  Serial.println(F("ESP8266 + BME680 starting up"));

  while (!bme.setup()) {
    Serial.println(F("[-] BME sensor setup failed"));
    delay(1000);
  }

  setup_wifi();
  setup_ntp();

  if (!influx_db.setup()) {
    Serial.println(F("[-] InfluxDB setup failed"));
    return;
  }

  Serial.println(F("[+] Setup completed successfully!"));
}

void loop() {
  influx_db.write_point(bme.read_tags(), bme.read_values());
  Serial.println("Waiting 1 second");

  delay(SAMPLE_INTERVAL_MS);
}