#include <MPU6050_light.h>
#include <Wire.h>
#include <cmath>

#include "set_up.h"
#include "wifi_and_communication.h"
#include "hammers.h"
#include "luna.h"
#include "vl53.h"
#include "mpu.h"
#include "rotation_and_display.h"

const int ITERATIONS = 100; // for debuging
unsigned long iterTimes[ITERATIONS]; // for debuging
int iterIndex = 0; // for debuging


void core0Fun(void *pvParameters) {

  TickType_t lastWakeTime = xTaskGetTickCount();
  const TickType_t period = pdMS_TO_TICKS(5);

  while (true) {
    unsigned long iterStart = micros();

    dataAll.CurrentTime = micros();

    //if (dataAll.CurrentTime - dataAll.PreviousTime >= 4800) {
    float dt = (dataAll.CurrentTime - dataAll.PreviousTime) / 1000000.0;
    dataAll.PreviousTime = dataAll.CurrentTime;
    readMPUValue();
    updateState(dataAll.GyroZ, dt);
    
    /* Used for debaging. if it is  not commented out it would hurt the performance, it prints the times for each iteration.
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
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {

  Serial.begin(115200);
  while (!Serial)
    delay(100);// pose until serial console opens
  initData();
  initWiFi();
  initLEDs();
  initHammers();
  
  //If one of the sensors is disconnected, its init function needs to be commented out for the code to continue.
  initMPU();
  //initLuna(); 
  initVl53();

  xTaskCreatePinnedToCore(
    core0Fun,      // Function
    "Core 0",   // Name
    8192,           // Stack size
    NULL,           // Parameters
    1,              // Priority
    NULL,           // Task handle
    0               // Core 0
  );
}

TickType_t loop_lastWakeTime = xTaskGetTickCount();
const TickType_t loop_period = pdMS_TO_TICKS(33);
void loop() {
  readAllVL();
  //readLidarValue();
  updateApp(dataAll);
  vTaskDelayUntil(&loop_lastWakeTime, loop_period);
}

