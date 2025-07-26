#include "./sensor.h"

#define BME680_DEBUG

BME680::BME680() {
  this->bme = new Adafruit_BME680(&Wire);
}

bool BME680::setup() {
  Serial.println(F("[*] Starting BME sensor setup"));

  int res = bme->begin();
  while (!res) {
    Serial.printf(
        "Could not find a valid BME680 sensor (result: %d), check wiring\n",
        res);
    return false;
  }
  // Set up oversampling and filter initialization
  bme->setTemperatureOversampling(BME680_OS_8X);
  bme->setHumidityOversampling(BME680_OS_2X);
  bme->setPressureOversampling(BME680_OS_4X);
  bme->setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme->setGasHeater(320, 150);  // 320*C for 150 ms

  Serial.println(F("[+] BME sensor setup successful!"));
  return true;
}

const char* BME680::get_device_name() {
  return DEVICE;
}

std::map<std::string, float> BME680::read_values() {
  std::map<std::string, float> values;
  if (!bme->performReading()) {
    Serial.println("Failed to perform reading ");
    delay(1000);
    return values;
  }

  // values["location"] = "living_room";
  // values["sensor_device"] = DEVICE;

  values["temperature (C)"] = bme->temperature;
  values["humidity (%)"] = bme->humidity;
  values["pressure (hPa)"] = bme->pressure;
  values["gas (KOhms)"] = bme->gas_resistance / 1000.0;
  values["altitude (m)"] = bme->readAltitude(SEALEVELPRESSURE_HPA);

  return values;
}