#include "vl53.h"

Adafruit_VL53L0X vl53[3];
VL53L0X_RangingMeasurementData_t measure[3];

const uint8_t vl53_addrs[3] = {VL1_ADDR, VL2_ADDR, VL3_ADDR};
const uint8_t xshut_pins[3] = {SHT1, SHT2, SHT3};

void initVl53() {
  for (int i = 0; i < 3; i++) {
    pinMode(xshut_pins[i], OUTPUT);
    digitalWrite(xshut_pins[i], LOW);
  }
  delay(10);

  // Start VL53s one-by-one with new address
  for (int i = 0; i < 3; i++) {
    digitalWrite(xshut_pins[i], HIGH);
    delay(10);
    if (!vl53[i].begin(vl53_addrs[i])) {
      //Serial.print("VL53 #");
      //Serial.print(i);
      // Serial.println(" fail");
      while (1);
    }
  }

  //Serial.println("All sensors initialized");
}

void readAllVL() {
  for (int i = 0; i < 3; i++) {
    vl53[i].rangingTest(&measure[i], false);  // false means no debug printing for individual sensors

    if (measure[i].RangeStatus == 0) {  // Status 0 means no error
      // Store the distance in the dataAll.vldistance array
      dataAll.vldistance[i] = measure[i].RangeMilliMeter;

      // Optionally print the distance
      //Serial.print("VL"); Serial.print(i);
      //Serial.print(": ");
      //Serial.print(dataAll.vldistance[i]);  // Print stored distance
      //Serial.println(" mm");
    } else {
      dataAll.vldistance[i] = -1;  // Set to -1 if out of range or error
      // Print error message
      //Serial.print("VL");
      //Serial.print(i);
      //Serial.println(": out of range");
    }
  }
}
