#ifndef Homie_Helper_H
#define Homie_Helper_H

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

void ota_setup(char* password);
void ota_handle();

void readSend(HomieNode* n, Data* d, Setting* s, double (*f)());

#endif
