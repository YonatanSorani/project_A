#ifndef SET_UP_H
#define SET_UP_H
#include <Arduino.h>

#define LEDS_NUM 8

enum LEDS {LED1 = 32, LED2 = 33, LED3 = 25, LED4 = 26, LED5 = 27, LED6 = 14, LED7 = 12, LED8 = 17};
extern int gpio_led[LEDS_NUM];


void initLEDs();
void initData();

struct Data {
  unsigned long CurrentTime;

  //mpu
  float mpuTemperature;
  float angle;
  float GyroZ;
  int rotationCount;
    
  //luna 
  int distanceLuna;
  int strengthLuna;
  float temperatureLuna;
  
  //VL
  int vldistance[3];
};
extern Data dataAll;

#endif