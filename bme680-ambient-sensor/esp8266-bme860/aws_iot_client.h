#ifndef IOT_H
#define IOT_H

#include <AwsIotWiFiClient.h>
#include "./iot_secrets.h"

class AWSIotClient {
private:
  AwsIotWiFiClient client;

public:
  AWSIotClient();
  void loop();
  void connect();
  void publish_message(const char *, const char *);
};

#endif  // IOT_H