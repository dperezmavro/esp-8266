#ifndef IOT_SECRETS_H
#define IOT_SECRETS_H

#define THING_NAME ""
#define AWS_IOT_PUBLISH_TOPIC ""

const char MQTT_HOST[] = "";

	
// Amazon Root CA 1
static const char cacert[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
-----END CERTIFICATE-----

)EOF";
 
// Copy contents from XXXXXXXX-certificate.pem.crt here ▼
static const char client_cert[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----
-----END CERTIFICATE-----

)KEY";
 
 
// Copy contents from  XXXXXXXX-private.pem.key here ▼
static const char privkey[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----
-----END RSA PRIVATE KEY-----

)KEY";

#endif