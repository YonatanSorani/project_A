#include "set_up.h"

// Define gpio_led array once in the .cpp file
int gpio_led[LEDS_NUM] = {LED1, LED2, LED3, LED4, LED5, LED6, LED7, LED8};

Data dataAll;

void initData()
{
    dataAll.CurrentTime = millis();
    dataAll.angle = 0;
    dataAll.GyroZ = 0;
    dataAll.rotationCount = 0;
    dataAll.mpuTemperature = 0;

    dataAll.distanceLuna = -1;
    dataAll.strengthLuna = 0;
    dataAll.temperatureLuna = 0;

    //vl
    dataAll.vldistance[0] = -1;
    dataAll.vldistance[1] = -1;
    dataAll.vldistance[2] = -1;
}


void initLEDs() {
    for (int i = 0; i < LEDS_NUM; ++i) {
        pinMode(gpio_led[i], OUTPUT);
        digitalWrite(gpio_led[i], HIGH);
    }
}
