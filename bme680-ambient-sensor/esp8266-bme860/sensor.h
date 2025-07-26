#ifndef _SENSOR_H
#define _SENSOR_H

#include <Adafruit_BME680.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <map>
#include <string>

#define DEVICE "bme680"
#define SEALEVELPRESSURE_HPA (1013.25)
#define SAMPLE_INTERVAL_MS 5000
#define OUTPUT_BUFFER_SIZE 256

class BME680 {
 private:
  Adafruit_BME680* bme;

 public:
  BME680();
  bool setup();
  const char* get_device_name();
  std::map<std::string, float> read_values();
};

#endif  // _SENSOR_H