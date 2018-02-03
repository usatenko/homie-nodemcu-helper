#include <Homie.h>
#include <Homie_Helper.h>
#include <ArduinoOTA.h>
#include <EEPROM.h>

long lastConnected = 0;
long lastDisconnected = millis();
bool restartInitiated = false;
void onHomieEvent(const HomieEvent& event) {
  switch(event.type) {
    case HomieEventType::MQTT_READY:
      // Do whatever you want when MQTT is connected in normal mode
      lastConnected = millis();
      break;
    case HomieEventType::MQTT_DISCONNECTED:
      // Do whatever you want when MQTT is disconnected in normal mode
      lastDisconnected = millis();
      // You can use event.mqttReason
      break;
  }
}

void pre_setup() {
  Homie.onEvent(onHomieEvent);
}

void post_setup(char* ota_password) {
  ArduinoOTA.setPassword(ota_password);
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

void pre_loop() {
}

void post_loop() {
  if (lastDisconnected > lastConnected && millis() - lastDisconnected > 10 * 1000 && !restartInitiated) {
    restartInitiated = true;
    Serial.println("Restarting");
    ESP.restart();
  }
  ArduinoOTA.handle();
}
void readSend(HomieNode& n, Data& d, Setting& s, THandlerFunction_Reader& r){
  if (millis() - d.lastRead >= s.intervalRead) {
    double v = r();
    if (!isnan(v) && d.vLast != v && d.vCurr == DBL_MAX) {
      d.vLast = v;
      d.vCurr = v;
    }
    if (millis() - d.lastSent >= s.intervalSend && d.vLast != DBL_MAX) {
      n.setProperty("value").send(String(d.vLast));
      d.lastSent = millis();
      d.vCurr = DBL_MAX;
    }
  }
}

String stateConvert(int v) {
  if (v != LOW && v != HIGH) {
    return "false";
  } else {
    return v == LOW ? "false" : "true";
  }
}

void writeSend(const HomieRange&, const String& value, HomieNode& n, Setting& s) {
  String val = value;
  if (val == "on") {
    stateConvert(EEPROM.read(s.eeprom));
  }
  if (val != "true" && val != "false") return;
  int on = (val == "true") ? HIGH : LOW;
  digitalWrite(s.pin, on);
  EEPROM.write(s.eeprom, on);
  EEPROM.commit();
  n.setProperty("state").send(val);
}
