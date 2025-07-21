#include "./aws_iot_client.h"

AWSIotClient::AWSIotClient() {
  trustAnchorCertificate = new BearSSL::X509List(cacert);
  clientCertificate = new BearSSL::X509List(client_cert);
  clientPrivateKey = new BearSSL::PrivateKey(privkey);

  client.setDebugOutput(true)
    .setCertificates(
      trustAnchorCertificate,
      clientCertificate,
      clientPrivateKey)
    .setEndpoint(MQTT_HOST)
    .setClientId(THING_NAME)
    .setSubscribeTopicFilter("DUMMY");
}

void AWSIotClient::publish_message(const char *topic, const char *contents) {
  client.publishMessage(topic, contents);
}

void AWSIotClient::loop() {
  client.loop();
}

void AWSIotClient::connect() {
  Serial.printf("number of certificates %d\n", trustAnchorCertificate->getCount());
  Serial.printf("number of certificates %d\n", clientCertificate->getCount());
  client.connect();
}