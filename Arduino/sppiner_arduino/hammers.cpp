#include "hammers.h"
#include <Arduino.h>

void motor_u2_left()
{
  // Rotate Motor Driver 2 - Motor A clockwise
  digitalWrite(M2_A_IA, HIGH);
  digitalWrite(M2_A_IB, LOW);

  // Rotate Motor Driver 2 - Motor B clockwise
  digitalWrite(M2_B_IA, HIGH);
  digitalWrite(M2_B_IB, LOW);
  //delay(500);
  //motor_u2_stop();
}

void motor_u2_right()
{
// Rotate Motor Driver 2 - Motor A counter-clockwise
  digitalWrite(M2_A_IA, LOW);
  digitalWrite(M2_A_IB, HIGH);

  // Rotate Motor Driver 2 - Motor B counter-clockwise
  digitalWrite(M2_B_IA, LOW);
  digitalWrite(M2_B_IB, HIGH);
  //delay(500);
  //motor_u2_stop();
  
}
void motor_u2_forward()
{
  // Rotate Motor Driver 2 - Motor A clockwise
  digitalWrite(M2_A_IA, HIGH);
  digitalWrite(M2_A_IB, LOW);

  // Rotate Motor Driver 2 - Motor B clockwise
  digitalWrite(M2_B_IA, LOW);
  digitalWrite(M2_B_IB, HIGH);
  //delay(500);
  //motor_u2_stop();
}
void motor_u2_backward()
{
  // Rotate Motor Driver 2 - Motor A clockwise
  digitalWrite(M2_A_IA, LOW);
  digitalWrite(M2_A_IB, HIGH);

  // Rotate Motor Driver 2 - Motor B clockwise
  digitalWrite(M2_B_IA, HIGH);
  digitalWrite(M2_B_IB, LOW);

  //delay(500);
  //motor_u2_stop();
}
void motor_u2_stop()
{
  // Rotate Motor Driver 2 - Motor A clockwise
  digitalWrite(M2_A_IA, LOW);
  digitalWrite(M2_A_IB, LOW);

  // Rotate Motor Driver 2 - Motor B clockwise
  digitalWrite(M2_B_IA, LOW);
  digitalWrite(M2_B_IB, LOW);
}


void initHammers()
{
  pinMode(M2_A_IA, OUTPUT);
  pinMode(M2_A_IB, OUTPUT);
  pinMode(M2_B_IA, OUTPUT);
  pinMode(M2_B_IB, OUTPUT);

  // Initialize all motor control pins to LOW
  digitalWrite(M2_A_IA, LOW);
  digitalWrite(M2_A_IB, LOW);
  digitalWrite(M2_B_IA, LOW);
  digitalWrite(M2_B_IB, LOW);
}