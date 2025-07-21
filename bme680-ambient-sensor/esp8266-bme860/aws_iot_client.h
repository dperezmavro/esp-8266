#ifndef _IOT_H
#define _IOT_H

#include <AwsIotWiFiClient.h>
#include "./iot_secrets.h"

class AWSIotClient {
private:
  AwsIotWiFiClient client;
  const BearSSL::X509List *trustAnchorCertificate;
  const BearSSL::X509List *clientCertificate;
  const BearSSL::PrivateKey *clientPrivateKey;

public:
  AWSIotClient();
  void loop();
  void connect();
  void publish_message(const char *, const char *);
};

#endif  // IOT_H