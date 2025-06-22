#include "vl53.h"

VL53L0X vl53[3];
const uint8_t vl53_addrs[3] = {VL1_ADDR, VL2_ADDR, VL3_ADDR};
const uint8_t xshut_pins[3] = {SHT1, SHT2, SHT3};

void initVl53() {
  for (int i = 0; i < 3; i++) {
    pinMode(xshut_pins[i], OUTPUT);
    digitalWrite(xshut_pins[i], LOW);
  }
  delay(10);
  for (int i = 0; i < 3; i++) {
    digitalWrite(xshut_pins[i], HIGH);
    delay(10);
    
    vl53[i].init();

    vl53[i].setAddress(vl53_addrs[i]); // Move AFTER init()
    vl53[i].setMeasurementTimingBudget(30000); // 30 ms budget (~20-33 Hz good)
    vl53[i].startContinuous(33);               // 33 ms interval (20–30 Hz actual)
  }
}

void readAllVL() {
  for (int i = 0; i < 3; i++) {
    uint16_t dist = vl53[i].readRangeContinuousMillimeters();
    //We limited the value so we could see it well in the graphs.
    if (vl53[i].timeoutOccurred() || dist > 1200) {
      dataAll.vldistance[i] = 120;
    } else {
      dataAll.vldistance[i] = dist/10;
    }
  }
}

