#ifndef Homie_Helper_H
#define Homie_Helper_H

#define DBL_MAX 1.7976931348623158e+308

typedef struct {
    byte pin;
    int intervalRead;
    int intervalSend;
    int eeprom;
} Setting;
typedef struct {
    double vLast;
    double vCurr;
    unsigned long lastRead;
    unsigned long lastSent;
} Data;
typedef std::function<double()> THandlerFunction_Reader;

void ota_setup(char* password);
void ota_handle();

void readSend(HomieNode& n, Data& d, Setting& s, THandlerFunction_Reader);
void writeSend(const HomieRange&, const String& val, HomieNode& n, Setting& s);
String stateConvert(int v);

#endif
