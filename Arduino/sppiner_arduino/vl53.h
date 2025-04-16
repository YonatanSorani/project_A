#ifndef VL53_H
#define VL53_H
#include <Arduino.h>
#include "Adafruit_VL53L0X.h"
#include "set_up.h"


#define VL1_ADDR 0x21
#define VL2_ADDR 0x22
#define VL3_ADDR 0x23

#define SHT1 13
#define SHT2 23
#define SHT3 15


void initVl53();
void readAllVL();

#endif