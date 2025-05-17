#include "rotation_and_display.h"

int previousSpinCount = -1; // Store the last spin count
unsigned long previousTime = 0;
unsigned long stoppedStartTime = 0; // Time when rotation stops
State currentState = STOPPED;


void updateState(float GyroZ, float dt) {
  switch (currentState) {
    case ROTATING:
      if (abs(GyroZ) < 0.1) { // Threshold to detect near-zero angular velocity
        if (stoppedStartTime == 0) {
          stoppedStartTime = millis(); // Start counting stationary time
        } else if (millis() - stoppedStartTime > stoppedThreshold) {
          currentState = STOPPED; // Transition to STOPPED state
          sendStopSpinningMassege();
          //Serial.println("State changed to STOPPED");
          break;
        }
      } else {
        stoppedStartTime = 0; // Reset the timer if motion resumes
      }
      handleRotatingState(GyroZ, dt);
      break;

    case STOPPED:
      if (abs(GyroZ) >= 0.1) { // Detect motion
        currentState = ROTATING;
        sendStartSpinningMassege();
        stoppedStartTime = 0; // Reset the stopped timer
        //Serial.println("State changed to ROTATING");
      }
      break;
  }
}

/*
//leds binary on spin count
void displaySpinCountOnLeds(int spinCount) {
  for (int i = 0; i < LEDS_NUM-1; i++) {
    // Check if the i-th bit of spinCount is 1
    if (spinCount & (1 << i)) {
      digitalWrite(gpio_led[i], LOW); // Turn LED on
    } else {
      digitalWrite(gpio_led[i], HIGH); // Turn LED off
    }
  }
}
*/



void handleRotatingState(float GyroZ, float dt) {
  // Update angle
  dataAll.angle += GyroZ * dt;

  // Check for full rotations
  if (dataAll.angle >= 2*M_PI) {
    dataAll.spinCount;
    dataAll.spinCount++;
    dataAll.angle -= 2*M_PI;
  } else if (dataAll.angle < 0) {
    //spinCount--;  //TODO:fix this case
    dataAll.angle += 2*M_PI;
  }
    // Display the number dynamically
  displayNumber(dataAll.spinCount, dataAll.angle); // Example: Display the number `0`

  // Update LEDs
  if (dataAll.spinCount != previousSpinCount) {
    previousSpinCount = dataAll.spinCount;
  }

}


////////////////////DISPLAY//////////////////////////



uint8_t rotatedPattern;
void displaySpinCountOnLeds(int number) {
  if (number < 0 || number > 9) return; // Validate the number
  // Apply the pattern to the LEDs
  for (int i = 0; i < LEDS_NUM-1; i++) {
    if (rotatedPattern & (1 << i)) {
      digitalWrite(gpio_led[i], LOW); // Turn LED ON
    } else {
      digitalWrite(gpio_led[i], HIGH); // Turn LED OFF
    }
  }
}

void display_0 (float angle){
    if(angle>=(0+0)*(M_PI/180) && angle <(5+0)*(M_PI/180)){
      rotatedPattern = 0b1111111;
    }
    else if(angle>=(5+0)*(M_PI/180) && angle <(25+0)*(M_PI/180)){
      rotatedPattern = 0b1000001;
    }
    else if(angle>=(25+0)*(M_PI/180) && angle <(30+0)*(M_PI/180)){
      rotatedPattern = 0b1111111;
    }
    else if(angle>=(30+0)*(M_PI/180) && angle <(90+0)*(M_PI/180)) {
    rotatedPattern = 0b0000000;
    }/*
    else if(angle>=(0+90)*(M_PI/180) && angle <(5+90)*(M_PI/180)){
      rotatedPattern = 0b1111111;
    }
    else if(angle>=(5+90)*(M_PI/180) && angle <(25+90)*(M_PI/180)){
      rotatedPattern = 0b1000001;
    }
    else if(angle>=(25+90)*(M_PI/180) && angle <(30+90)*(M_PI/180)){
      rotatedPattern = 0b1111111;
    }
    else if(angle>=(30+90)*(M_PI/180) && angle <(90+90)*(M_PI/180)) {
    rotatedPattern = 0b0000000;
    }
    else if(angle>=(0+180)*(M_PI/180) && angle <(5+180)*(M_PI/180)){
      rotatedPattern = 0b1111111;
    }
    else if(angle>=(5+180)*(M_PI/180) && angle <(25+180)*(M_PI/180)){
      rotatedPattern = 0b1000001;
    }
    else if(angle>=(25+180)*(M_PI/180) && angle <(30+180)*(M_PI/180)){
      rotatedPattern = 0b1111111;
    }*/
    else{
    rotatedPattern = 0b0000000;
    } 
  displaySpinCountOnLeds(0);
}

void display_1 (float angle){
    if(angle>=(0+0)*(M_PI/180) && angle <(5+0)*(M_PI/180)){
    rotatedPattern = 0b1111111;
    }
    else if(angle>=(5+0)*(M_PI/180) && angle <(0+10)*(M_PI/180)){
    rotatedPattern = 0b0000001;
    }/*
    if(angle>=(0+90)*(M_PI/180) && angle <(5+90)*(M_PI/180)){
    rotatedPattern = 0b1111111;
    }
    else if(angle>=(5+90)*(M_PI/180) && angle <(90+10)*(M_PI/180)){
    rotatedPattern = 0b0000001;
    }
    if(angle>=(0+180)*(M_PI/180) && angle <(5+180)*(M_PI/180)){
    rotatedPattern = 0b1111111;
    }*/
    else {
    rotatedPattern = 0b0000000;
    }
  displaySpinCountOnLeds(1);
}

void display_2 (float angle){
    if(angle>=(0+0)*(M_PI/180) && angle <(5+0)*(M_PI/180)){
      rotatedPattern = 0b1001111;
    }
    else if(angle>=(5+0)*(M_PI/180) && angle <(25+0)*(M_PI/180)){
      rotatedPattern = 0b1001001;
    }
    else if(angle>=(25+0)*(M_PI/180) && angle <(30+0)*(M_PI/180)){
      rotatedPattern = 0b1111001;
    }
    else if(angle>=(30+0)*(M_PI/180) && angle <(90+0)*(M_PI/180)) {
    rotatedPattern = 0b0000000;
    }/*
    else if(angle>=(0+90)*(M_PI/180) && angle <(5+90)*(M_PI/180)){
      rotatedPattern = 0b1001111;
    }
    else if(angle>=(5+90)*(M_PI/180) && angle <(25+90)*(M_PI/180)){
      rotatedPattern = 0b1001001;
    }
    else if(angle>=(25+90)*(M_PI/180) && angle <(30+90)*(M_PI/180)){
      rotatedPattern = 0b1111001;
    }
    else if(angle>=(30+90)*(M_PI/180) && angle <(90+90)*(M_PI/180)) {
    rotatedPattern = 0b0000000;
    }
    else if(angle>=(0+180)*(M_PI/180) && angle <(5+180)*(M_PI/180)){
      rotatedPattern = 0b1001111;
    }
    else if(angle>=(5+180)*(M_PI/180) && angle <(25+180)*(M_PI/180)){
      rotatedPattern = 0b1001001;
    }
    else if(angle>=(25+180)*(M_PI/180) && angle <(30+180)*(M_PI/180)){
      rotatedPattern = 0b1111001;
    }*/
    else{
    rotatedPattern = 0b0000000;
    }  
  displaySpinCountOnLeds(2);
}

void display_3 (float angle){
    if(angle>=(0+0)*(M_PI/180) && angle <(5+0)*(M_PI/180)){
      rotatedPattern = 0b1111111;
    }
    else if(angle>=(5+0)*(M_PI/180) && angle <(30+0)*(M_PI/180)){
      rotatedPattern = 0b1001001;
    }
    else if(angle>=(30+0)*(M_PI/180) && angle <(90+0)*(M_PI/180)){
      rotatedPattern = 0b0000000;
    }/*
    else if(angle>=(0+90)*(M_PI/180) && angle <(5+90)*(M_PI/180)){
      rotatedPattern = 0b1111111;
    }
    else if(angle>=(5+90)*(M_PI/180) && angle <(30+90)*(M_PI/180)){
      rotatedPattern = 0b1001001;
    }
    else if(angle>=(30+90)*(M_PI/180) && angle <(90+90)*(M_PI/180)){
      rotatedPattern = 0b0000000;
    }
    else if(angle>=(0+180)*(M_PI/180) && angle <(5+180)*(M_PI/180)){
      rotatedPattern = 0b1111111;
    }
    else if(angle>=(5+180)*(M_PI/180) && angle <(30+180)*(M_PI/180)){
      rotatedPattern = 0b1001001;
    }*/
    else{
    rotatedPattern = 0b0000000;
    }  
  displaySpinCountOnLeds(3);
}

void display_4 (float angle){
    if(angle>=(0+0)*(M_PI/180) && angle <(5+0)*(M_PI/180)){
      rotatedPattern = 0b1111111;
    }
    else if(angle>=(5+0)*(M_PI/180) && angle <(25+0)*(M_PI/180)){
      rotatedPattern = 0b0001000;
    }
    else if(angle>=(25+0)*(M_PI/180) && angle <(30+0)*(M_PI/180)){
      rotatedPattern = 0b0001111;
    }/*
    else if(angle>=(30+0)*(M_PI/180) && angle <(90+0)*(M_PI/180)) {
    rotatedPattern = 0b0000000;
    }
    else if(angle>=(0+90)*(M_PI/180) && angle <(5+90)*(M_PI/180)){
      rotatedPattern = 0b1111111;
    }
    else if(angle>=(5+90)*(M_PI/180) && angle <(25+90)*(M_PI/180)){
      rotatedPattern = 0b0001000;
    }
    else if(angle>=(25+90)*(M_PI/180) && angle <(30+90)*(M_PI/180)){
      rotatedPattern = 0b0001111;
    }
    else if(angle>=(30+90)*(M_PI/180) && angle <(90+90)*(M_PI/180)) {
    rotatedPattern = 0b0000000;
    }
    else if(angle>=(0+180)*(M_PI/180) && angle <(5+180)*(M_PI/180)){
      rotatedPattern = 0b1111111;
    }
    else if(angle>=(5+180)*(M_PI/180) && angle <(25+180)*(M_PI/180)){
      rotatedPattern = 0b0001000;
    }
    else if(angle>=(25+180)*(M_PI/180) && angle <(30+180)*(M_PI/180)){
      rotatedPattern = 0b0001111;
    }*/
    else{
    rotatedPattern = 0b0000000;
    }  
  displaySpinCountOnLeds(4);
}

void display_5 (float angle){
    if(angle>=(0+0)*(M_PI/180) && angle <(5+0)*(M_PI/180)){
      rotatedPattern = 0b1111001;
    }
    else if(angle>=(5+0)*(M_PI/180) && angle <(25+0)*(M_PI/180)){
      rotatedPattern = 0b1001001;
    }
    else if(angle>=(25+0)*(M_PI/180) && angle <(30+0)*(M_PI/180)){
      rotatedPattern = 0b1001111;
    }
    else if(angle>=(30+0)*(M_PI/180) && angle <(90+0)*(M_PI/180)) {
    rotatedPattern = 0b0000000;
    }/*
    else if(angle>=(0+90)*(M_PI/180) && angle <(5+90)*(M_PI/180)){
      rotatedPattern = 0b1111001;
    }
    else if(angle>=(5+90)*(M_PI/180) && angle <(25+90)*(M_PI/180)){
      rotatedPattern = 0b1001001;
    }
    else if(angle>=(25+90)*(M_PI/180) && angle <(30+90)*(M_PI/180)){
      rotatedPattern = 0b1001111;
    }
    else if(angle>=(30+90)*(M_PI/180) && angle <(90+90)*(M_PI/180)) {
    rotatedPattern = 0b0000000;
    }
    else if(angle>=(0+180)*(M_PI/180) && angle <(5+180)*(M_PI/180)){
      rotatedPattern = 0b1111001;
    }
    else if(angle>=(5+180)*(M_PI/180) && angle <(25+180)*(M_PI/180)){
      rotatedPattern = 0b1001001;
    }
    else if(angle>=(25+180)*(M_PI/180) && angle <(30+180)*(M_PI/180)){
      rotatedPattern = 0b1001111;
    }*/
    else{
    rotatedPattern = 0b0000000;
    }  
  displaySpinCountOnLeds(5);
}

void display_6 (float angle){
    if(angle>=(0+0)*(M_PI/180) && angle <(5+0)*(M_PI/180)){
      rotatedPattern = 0b1111001;
    }
    else if(angle>=(5+0)*(M_PI/180) && angle <(25+0)*(M_PI/180)){
      rotatedPattern = 0b1001001;
    }
    else if(angle>=(25+0)*(M_PI/180) && angle <(30+0)*(M_PI/180)){
      rotatedPattern = 0b1111111;
    }
    else if(angle>=(30+0)*(M_PI/180) && angle <(90+0)*(M_PI/180)) {
    rotatedPattern = 0b0000000;
    }/*
    else if(angle>=(0+90)*(M_PI/180) && angle <(5+90)*(M_PI/180)){
      rotatedPattern = 0b1111001;
    }
    else if(angle>=(5+90)*(M_PI/180) && angle <(25+90)*(M_PI/180)){
      rotatedPattern = 0b1001001;
    }
    else if(angle>=(25+90)*(M_PI/180) && angle <(30+90)*(M_PI/180)){
      rotatedPattern = 0b1111111;
    }
    else if(angle>=(30+90)*(M_PI/180) && angle <(90+90)*(M_PI/180)) {
    rotatedPattern = 0b0000000;
    }
    else if(angle>=(0+180)*(M_PI/180) && angle <(5+180)*(M_PI/180)){
      rotatedPattern = 0b1111001;
    }
    else if(angle>=(5+180)*(M_PI/180) && angle <(25+180)*(M_PI/180)){
      rotatedPattern = 0b1001001;
    }
    else if(angle>=(25+180)*(M_PI/180) && angle <(30+180)*(M_PI/180)){
      rotatedPattern = 0b1111111;
    }*/
    else{
    rotatedPattern = 0b0000000;
    }  
  displaySpinCountOnLeds(6);
}

void display_7 (float angle){
    if(angle>=(0+0)*(M_PI/180) && angle <(5+0)*(M_PI/180)){
      rotatedPattern = 0b1111111;
    }
    else if(angle>=(5+0)*(M_PI/180) && angle <(30+0)*(M_PI/180)){
      rotatedPattern = 0b0000001;
    }
    else if(angle>=(30+0)*(M_PI/180) && angle <(90+0)*(M_PI/180)){
      rotatedPattern = 0b0000000;
    }/*
    else if(angle>=(0+90)*(M_PI/180) && angle <(5+90)*(M_PI/180)){
      rotatedPattern = 0b1111111;
    }
    else if(angle>=(5+90)*(M_PI/180) && angle <(30+90)*(M_PI/180)){
      rotatedPattern = 0b0000001;
    }
    else if(angle>=(30+90)*(M_PI/180) && angle <(90+90)*(M_PI/180)){
      rotatedPattern = 0b0000000;
    }
    else if(angle>=(0+180)*(M_PI/180) && angle <(5+180)*(M_PI/180)){
      rotatedPattern = 0b1111111;
    }
    else if(angle>=(5+180)*(M_PI/180) && angle <(30+180)*(M_PI/180)){
      rotatedPattern = 0b0000001;
    }*/
    else{
    rotatedPattern = 0b0000000;
    }  
  displaySpinCountOnLeds(7);
}

void display_8 (float angle){
    if(angle>=(0+0)*(M_PI/180) && angle <(5+0)*(M_PI/180)){
      rotatedPattern = 0b1111111;
    }
    else if(angle>=(5+0)*(M_PI/180) && angle <(25+0)*(M_PI/180)){
      rotatedPattern = 0b1001001;
    }
    else if(angle>=(25+0)*(M_PI/180) && angle <(30+0)*(M_PI/180)){
      rotatedPattern = 0b1111111;
    }
    else if(angle>=(30+0)*(M_PI/180) && angle <(90+0)*(M_PI/180)) {
    rotatedPattern = 0b0000000;
    }/*
    else if(angle>=(0+90)*(M_PI/180) && angle <(5+90)*(M_PI/180)){
      rotatedPattern = 0b1111111;
    }
    else if(angle>=(5+90)*(M_PI/180) && angle <(25+90)*(M_PI/180)){
      rotatedPattern = 0b1001001;
    }
    else if(angle>=(25+90)*(M_PI/180) && angle <(30+90)*(M_PI/180)){
      rotatedPattern = 0b1111111;
    }
    else if(angle>=(30+90)*(M_PI/180) && angle <(90+90)*(M_PI/180)) {
    rotatedPattern = 0b0000000;
    }
    else if(angle>=(0+180)*(M_PI/180) && angle <(5+180)*(M_PI/180)){
      rotatedPattern = 0b1111111;
    }
    else if(angle>=(5+180)*(M_PI/180) && angle <(25+180)*(M_PI/180)){
      rotatedPattern = 0b1001001;
    }
    else if(angle>=(25+180)*(M_PI/180) && angle <(30+180)*(M_PI/180)){
      rotatedPattern = 0b1111111;
    }*/
    else{
    rotatedPattern = 0b0000000;
    }  
  displaySpinCountOnLeds(8);
}

void display_9 (float angle){
    if(angle>=(0+0)*(M_PI/180) && angle <(5+0)*(M_PI/180)){
      rotatedPattern = 0b1111111;
    }
    else if(angle>=(5+0)*(M_PI/180) && angle <(25+0)*(M_PI/180)){
      rotatedPattern = 0b1001001;
    }
    else if(angle>=(25+0)*(M_PI/180) && angle <(30+0)*(M_PI/180)){
      rotatedPattern = 0b1001111;
    }
    else if(angle>=(30+0)*(M_PI/180) && angle <(90+0)*(M_PI/180)) {
    rotatedPattern = 0b0000000;
    }/*
    else if(angle>=(0+90)*(M_PI/180) && angle <(5+90)*(M_PI/180)){
      rotatedPattern = 0b1111111;
    }
    else if(angle>=(5+90)*(M_PI/180) && angle <(25+90)*(M_PI/180)){
      rotatedPattern = 0b1001001;
    }
    else if(angle>=(25+90)*(M_PI/180) && angle <(30+90)*(M_PI/180)){
      rotatedPattern = 0b1001111;
    }
    else if(angle>=(30+90)*(M_PI/180) && angle <(90+90)*(M_PI/180)) {
    rotatedPattern = 0b0000000;
    }
    else if(angle>=(0+180)*(M_PI/180) && angle <(5+180)*(M_PI/180)){
      rotatedPattern = 0b1111111;
    }
    else if(angle>=(5+180)*(M_PI/180) && angle <(25+180)*(M_PI/180)){
      rotatedPattern = 0b1001001;
    }
    else if(angle>=(25+180)*(M_PI/180) && angle <(30+180)*(M_PI/180)){
      rotatedPattern = 0b1001111;
    }*/
    else{
    rotatedPattern = 0b0000000;
    }   
  displaySpinCountOnLeds(9);
}

void displayNumber(int number, float angle) {
  switch ((number/10)%10) {
    case 0:
      display_0(angle);
      break;
    case 1:
      display_1(angle);
      break;
    case 2:
      display_2(angle);
      break;
    case 3:
      display_3(angle);
      break;
    case 4:
      display_4(angle);
      break;
    case 5:
      display_5(angle);
      break;
    case 6:
      display_6(angle);
      break;
    case 7:
      display_7(angle);
      break;
    case 8:
      display_8(angle);
      break;
    case 9:
      display_9(angle);
      break;    
    default:
      display_9(angle);
      break;
  }
}


