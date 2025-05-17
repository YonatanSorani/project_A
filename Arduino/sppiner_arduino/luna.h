#ifndef LUNA_H
#define LUNA_H
#include <Arduino.h>
#include "set_up.h"

#define RXD2 16 // LUNA_ESP_RXD
#define TXD2 17 // ESP_LUNA_LED8_TXD

void readLidarValue();
void initLuna();


#endif