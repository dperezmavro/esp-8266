#ifndef _INFLUX_DB_H
#define _INFLUX_DB_H


#include <InfluxDbClient.h>
#include <InfluxDbCloud.h>
#include <map>
#include <string>

#include "influx_db_secrets.h"

#define INFLUXDB_BUCKET "bme680_sensor"

// Time zone info
#define TZ_INFO "UTC0"

class InfluxDB {
private:
  InfluxDBClient* influx_client;
  Point* sensor;
public:
  InfluxDB(const char* point_name);
  bool write_point(
    const std::map<std::string, float>& values);
  bool setup();
};


#endif