#include "./setup.h"

#include "./wifi.h"

void setup_wifi()
{
  Serial.println(F("[*] Starting WiFi setup"));
  WiFi.begin(wifi_ssid, wifi_pass);

  Serial.println("[*] Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1500);
    Serial.printf("  last status: %d.\n", WiFi.status());
  }
  Serial.printf("[+] Connected, IP address: %s\n",
                WiFi.localIP().toString().c_str());
  Serial.println(F("[+] WiFi setup successful!"));
}

void setup_ntp()
{
  Serial.println(F("[*] Starting NTP setup"));
  timeSync(TZ_INFO, "pool.ntp.org", "time.nis.gov");
  Serial.println(F("[+] NTP setup successful!"));
}
