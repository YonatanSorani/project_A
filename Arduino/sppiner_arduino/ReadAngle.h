#include "Wire.h"
#include <MPU6050_light.h>
#include <iostream>
#include <iterator>
#include <map>
#include <queue>
#include <vector>
#include <sstream>
#include <algorithm>
#include "SPIFFS.h"

#define BUILT_IN_LED 2

#define RXD2 16
#define TXD2 17

#define GYROZ_THRESHOLD_FOR_ROTATION_START 50 // deg/sec
#define GYROZ_THRESHOLD_FOR_ROTATION_STOP 5

#define MAX_BEACON_DISTANCE 30
#define DISPLAY_ARROW true
//configuration should either be MAPPING or LEDS_DISPLAY
#define CONFIGURATION_MAPPING true
#define CONFIGURATION_LEDS_DISPLAY false


using namespace std;

void Wrapping_OffsetCorrection();
 
enum CurrentState
{
    state_mapping,
    state_waiting_to_rotate,
    state_waiting_to_beacon,
    state_rotating,
};
struct ReadAngle{
	
	float currentNormalizedAngleZ = 0;
	float currentNormalizedAngleZWithOffset;
	CurrentState currentState = state_waiting_to_rotate;

	float currentAngleZOffset = 0;

  //luna vars
  int currentLunaDistance = 0;

};
