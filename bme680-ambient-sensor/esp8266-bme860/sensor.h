#ifndef _SENSOR_H
#define _SENSOR_H

#include <Adafruit_Sensor.h>
#include <Wire.h>

#include <map>
#include <string>

#include "Adafruit_BME680.h"

#define DEVICE "bme680"
#define SEALEVELPRESSURE_HPA (1013.25)

class BME680
{
 private:
  Adafruit_BME680* bme;
  std::map<std::string, std::string> tags;

 public:
  BME680(std::string location);
  bool setup();
  const char* get_device_name();
  std::map<std::string, float> read_values();
  std::map<std::string, std::string> read_tags();
};

#endif  // _SENSOR_H