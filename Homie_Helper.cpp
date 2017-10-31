#include <Homie_Helper.h>
#include <ArduinoOTA.h>

void ota_setup(char* password, byte led) {
  ArduinoOTA.setPassword(password);
  ArduinoOTA.onStart([]() {});
  ArduinoOTA.onEnd([]() {
    digitalWrite(led, LOW);
    for (int i = 0; i < 20; i++)
       {
         digitalWrite(led, HIGH);
         delay(i * 2);
         digitalWrite(led, LOW);
         delay(i * 2);
       }
       digitalWrite(led, HIGH);
       ESP.restart();
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    digitalWrite(led, total % 1);
  });
  ArduinoOTA.onError([](ota_error_t error) {
    if (error == OTA_AUTH_ERROR);          // Auth failed
    else if (error == OTA_BEGIN_ERROR);    // Begin failed
    else if (error == OTA_CONNECT_ERROR);  // Connect failed
    else if (error == OTA_RECEIVE_ERROR);  // Receive failed
    else if (error == OTA_END_ERROR);      // End failed
  });
  ArduinoOTA.begin();
}