#include "./sensor.h"

#define BME680_DEBUG

#define BME680_I2C_ADDRESS 0x77
#define I2C_SDA 12  // D6
#define I2C_SCL 14  // D5

BME680::BME680(std::string location)
{
  Wire.begin(I2C_SDA, I2C_SCL);
  this->bme = new Adafruit_BME680(&Wire);

  tags["location"]      = location;
  tags["sensor_device"] = DEVICE;
}

bool BME680::setup()
{
  Serial.println(F("[*] Starting BME sensor setup"));

  int res = bme->begin(BME680_I2C_ADDRESS);
  if (!res)
  {
    Serial.printf(
        "Could not find a valid BME680 sensor (result: %d), check wiring\n",
        res);
    return false;
  }
  // Set up oversampling and filter initialization
  bme->setTemperatureOversampling(BME680_OS_8X);
  bme->setHumidityOversampling(BME680_OS_2X);
  bme->setPressureOversampling(BME680_OS_4X);
  // bme->setIIRFilterSize(BME680_FILTER_SIZE_3);
  // bme->setGasHeater(320, 150);  // 320*C for 150 ms

  Serial.println(F("[+] BME sensor setup successful!"));
  return true;
}

const char* BME680::get_device_name()
{
  return DEVICE;
}

std::map<std::string, float> BME680::read_values()
{
  std::map<std::string, float> sensor_readings;
  if (!bme->performReading())
  {
    Serial.println("Failed to perform reading ");
    delay(1000);
    return sensor_readings;
  }

  sensor_readings["temperature"] = bme->temperature;
  sensor_readings["humidity"]    = bme->humidity;
  sensor_readings["pressure"]    = bme->pressure;
  sensor_readings["gas"]         = bme->gas_resistance / 1000.0;
  sensor_readings["altitude"]    = bme->readAltitude(SEALEVELPRESSURE_HPA);

  return sensor_readings;
}

std::map<std::string, std::string> BME680::read_tags()
{
  return tags;
}