#include "./influx_db.h"

InfluxDB::InfluxDB(const char* point_name) {
  this->influx_client =
      new InfluxDBClient(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET,
                         INFLUXDB_TOKEN, InfluxDbCloud2CACert);
  this->point = new Point(point_name);
}

bool InfluxDB::setup() {
  Serial.println(F("[*] Starting InfluxDB setup"));
  if (!influx_client->validateConnection()) {
    Serial.print("InfluxDB connection failed: ");
    Serial.println(influx_client->getLastErrorMessage());
    return false;
  }
  Serial.println(F("[+] InfluxDB setup successful!"));
  return true;
}

bool InfluxDB::write_point(const std::map<std::string, std::string>& tags,
                           const std::map<std::string, float>& values) {
  // Clear any existing fields
  point->clearFields();

  // Add all values from the hashset as fields
  for (const auto& pair : values) {
    point->addField(pair.first.c_str(), pair.second);
  }

  // TODO(dio): this is buggy
  // for (const auto& pair : tags) {
  //   point->addField(pair.first.c_str(), pair.second.c_str());
  // }

  // Write the point to InfluxDB
  if (!influx_client->writePoint(*point)) {
    Serial.print("InfluxDB write failed: ");
    Serial.println(influx_client->getLastErrorMessage());
    return false;
  }

  return true;
}