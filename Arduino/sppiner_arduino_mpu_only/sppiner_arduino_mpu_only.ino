#include <MPU6050_light.h>
#include <Wire.h>

#include <cmath>

#include "set_up.h"
#include "wifi_and_communication.h"

#include "mpu.h"
#include "rotation_and_display.h"

const int ITERATIONS = 100;
unsigned long iterTimes[ITERATIONS]; // for debuging
int iterIndex = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {

  Serial.begin(115200);
  while (!Serial)
    delay(100);// pose until serial console opens
  initData();
  //initWiFi();
  initLEDs();
  initMPU();
}

TickType_t lastWakeTime = xTaskGetTickCount();
const TickType_t period = pdMS_TO_TICKS(5);
void loop() {

  unsigned long iterStart = micros();

  dataAll.CurrentTime = micros();

  //if (dataAll.CurrentTime - dataAll.PreviousTime >= 4800) {
  float dt = (dataAll.CurrentTime - dataAll.PreviousTime) / 1000000.0;
  dataAll.PreviousTime = dataAll.CurrentTime;
  readMPUValue();
  updateState(dataAll.GyroZ, dt);
    /*
  unsigned long iterEnd = micros();
  unsigned long duration = iterEnd - iterStart;

  iterTimes[iterIndex++] = duration;

  if (iterIndex >= ITERATIONS) {
    unsigned long sum = 0;
    for (int i = 0; i < ITERATIONS; i++) {
      Serial.print("Iter ");
      Serial.print(i);
      Serial.print(": ");
      Serial.println(iterTimes[i]);
      sum += iterTimes[i];
    }
    Serial.print("Average iteration time (µs): ");
    Serial.println(sum / ITERATIONS);
    iterIndex = 0;
  }
  */
  vTaskDelayUntil(&lastWakeTime, period);
}

