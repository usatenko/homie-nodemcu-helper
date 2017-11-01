#include <Homie.h>
#include <Homie_Helper.h>
#include <ArduinoOTA.h>

void ota_setup(char* password) {
  ArduinoOTA.setPassword(password);
  ArduinoOTA.onStart([]() {});
  ArduinoOTA.onEnd([]() {
    digitalWrite(D0, LOW);
    for (int i = 0; i < 20; i++)
       {
         digitalWrite(D0, HIGH);
         delay(i * 2);
         digitalWrite(D0, LOW);
         delay(i * 2);
       }
       digitalWrite(D0, HIGH);
       ESP.restart();
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    digitalWrite(D0, total % 1);
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

void ota_handle() {
  ArduinoOTA.handle();
}
void readSend(HomieNode* n, Data* d, Setting* s, double (*f)()){
    if (millis() - d->lastRead >= s->intervalRead) {
        double v = (*f)();
        if (!isnan(v) && v != d->vLast && millis() - d->lastSent >= s->intervalSend) {
          d->vLast = v;
          n->setProperty("value").send(String(v));
          d->lastSent = millis();
        }
    }
}
