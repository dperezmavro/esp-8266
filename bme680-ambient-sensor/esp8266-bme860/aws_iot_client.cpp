#include "./aws_iot_client.h"

AWSIotClient::AWSIotClient() {
  BearSSL::X509List trustAnchorCertificate(cacert);
  BearSSL::X509List clientCertificate(client_cert);
  BearSSL::PrivateKey clientPrivateKey(privkey);

  client.setDebugOutput(false)
    .setCertificates(
      &trustAnchorCertificate,
      &clientCertificate,
      &clientPrivateKey)
    .setEndpoint(MQTT_HOST)
    .setSubscribeTopicFilter("DUMMY")
    .setClientId(THING_NAME);
}

void AWSIotClient::publish_message(const char *topic, const char *contents) {
  client.publishMessage(topic, contents);
}

void AWSIotClient::loop() {
  client.loop();
}

void AWSIotClient::connect() {
  client.connect();
}