#ifndef ROTATION_AND_DISPLAY_H
#define ROTATION_AND_DISPLAY_H
#include "set_up.h"
#include "wifi_and_communication.h"

enum State { ROTATING, STOPPED };
const unsigned long stoppedThreshold = 100; // 0.1 seconds threshold

void updateState(float GyroZ, float dt);
void handleRotatingState(float GyroZ, float dt);
void displayNumber(int number, float angle);

#endif