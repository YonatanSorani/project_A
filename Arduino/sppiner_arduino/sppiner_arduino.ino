#include <MPU6050_light.h>
#include <Wire.h>
#include "ReadAngle.h"
#include <cmath>

#include "set_up.h"
#include "wifi_and_communication.h"
#include "hammers.h"
#include "luna.h"
#include "vl53.h"

#include <Ticker.h>
Ticker ticker;
volatile bool doSend = false;


using namespace std;


//real time pro
bool next_round;
bool next_round_90deg;
bool next_round_neg90deg;


unsigned long previousTime = 0;
unsigned long CurrentTime_90deg;
unsigned long previousTime_90deg = 0;
unsigned long CurrentTime_neg90deg;
unsigned long previousTime_neg90deg = 0;
int16_t TimeAngle;
//--------------------all variable for leds control START----------------------
unsigned int delay1 = 100;
int ch;
unsigned long preInterval;

uint8_t  hours = 00;
uint8_t  minutes = 00;
uint8_t  seconds = 00;

//this parameter is going to be determined based on experiements
//started form 90 - TODO: write results before changing to another size so you add these results in the report 
int Length = 70;//90 degree is the size of word that will be displayed
float Speed = GYROZ_THRESHOLD_FOR_ROTATION_START * (57.29578);//~180/pi// angular velocity, after converting units to [deg/sec]  
float Time;

//------------------------------------------END--------------------------------
unsigned long lastTime = 0;  
unsigned long timerDelay = 300;

float gyroSensorVal = 0;
//Create a sensor object
MPU6050 mpu = MPU6050(Wire);
float GyroX, GyroY, GyroZ;
float InitGyroX, InitGyroY, InitGyroZ;
float mpuTemperature, lunaTemperature;
//float GyroErrorX = 0, GyroErrorY = 0, GyroErrorZ = 0;
int c = 0;


ReadAngle angle_var;
vector<int> mapping(360, 0);
CurrentState currentState = state_waiting_to_rotate;



void getSensorReadings(){
  mpu.update();
  GyroZ = mpu.getGyroZ();
  mpuTemperature = mpu.getTemp();
}

//vars for loop
//char general_output[200];
int currentEstimatedAngle = 0;//מקבל float
int avg_luna_distance;
int sum_luna_distance = 0;
bool Beacon_ahead = false;

///////////////////////////////////////////////////////////////////////////////////////////////////



void ticker_fun()
{
  doSend = true;
}

void core0Fun (void *pvParameters)
{
  while (true) {
    vTaskDelay(100 / portTICK_PERIOD_MS); // Prevent watchdog reset
    readAllVL();
    if (Serial2.available() > 0) {
      Serial.println("connected to lidar");
      readLidarValue();   
    
      if (angle_var.currentLunaDistance <= MAX_BEACON_DISTANCE && angle_var.currentLunaDistance > 0)
      {
       
          Beacon_ahead = true;
          //Counter_See_Beacon += 1;
          sum_luna_distance += angle_var.currentLunaDistance;
          //avg_luna_distance = round(sum_luna_distance/Counter_See_Beacon);
          Serial.println("currentLunaDistance <= BEACON_DISTANCE");
          angle_var.currentAngleZOffset = angle_var.currentNormalizedAngleZ;
          angle_var.currentAngleZOffset = fmod(angle_var.currentAngleZOffset, 360.0);
          if (angle_var.currentAngleZOffset < 0)
          {
               angle_var.currentAngleZOffset += 360.0;
          }
          currentEstimatedAngle = 0;
      }
    }
    updateGraph(dataAll,mpuTemperature,GyroZ,currentEstimatedAngle);

     
  }



}
////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(115200);
  while (!Serial)
    delay(100);// pose until serial console opens

  initWiFi();
  initLEDs();
  initHammers();
  initData();
  initLuna(); 
  initVl53();





  // Boot MPU6050
  // For I2C
  Wire.begin();          
  uint8_t  status = mpu.begin(3, 3);//parameters are (gyro_config_num,acc_config_num) 
  //gyro_config_num is what matter for the project - angular velocity 
  //for gyro_config_num = 0, range = +- 250 deg/s
  //the smaller the range the more sensitive the gyroscope!
  //this big range might cause a lot of noise in the measurement
  
  Serial.print(F("MPU6050 status: "));
  Serial.println(status);
  if (status != 0) // stop everything if could not connect to MPU6050
  {
      Serial.println(F("Failed to boot MPU6050."));
      while (1)
          ;
  }
  Serial.println(F("Calculating offsets, do not move MPU6050"));
  //this takes 500 ms to be done
  mpu.calcOffsets();
  Serial.println(F("Done!\n"));


  //set time
  previousTime = millis();
  next_round = true;

  //lighting in angle 90 parameters:
  previousTime_90deg = millis();
  CurrentTime_90deg = millis();
  next_round_90deg = true;

  //lighting in angle -90 parameters:
  previousTime_neg90deg = millis();
  CurrentTime_neg90deg = millis();
  next_round_neg90deg = true;

  
  preInterval = millis();


  xTaskCreatePinnedToCore(
    core0Fun,      // Function
    "Core 0",   // Name
    4096,           // Stack size
    NULL,           // Parameters
    1,              // Priority
    NULL,           // Task handle
    0               // Core 0
  );
}

void loop() {
  delay(5);
  dataAll.CurrentTime = millis();
  CurrentTime_90deg = millis();
  getSensorReadings();
  //if(currentState != state_waiting_to_rotate){
    unsigned long Tnew = dataAll.CurrentTime;
    float dt = (Tnew - preInterval) * 1e-3;
    preInterval = Tnew;
    angle_var.currentNormalizedAngleZ += GyroZ*dt;
    Wrapping_OffsetCorrection();
    currentEstimatedAngle = angle_var.currentNormalizedAngleZWithOffset;
  //}
  /*
  readAllVL();
    if (Serial2.available() > 0) {
      Serial.println("connected to lidar");
      readLidarValue();   
    
      if (angle_var.currentLunaDistance <= MAX_BEACON_DISTANCE && angle_var.currentLunaDistance > 0)
      {
       
          Beacon_ahead = true;
          //Counter_See_Beacon += 1;
          sum_luna_distance += angle_var.currentLunaDistance;
          //avg_luna_distance = round(sum_luna_distance/Counter_See_Beacon);
          Serial.println("currentLunaDistance <= BEACON_DISTANCE");
          angle_var.currentAngleZOffset = angle_var.currentNormalizedAngleZ;
          angle_var.currentAngleZOffset = fmod(angle_var.currentAngleZOffset, 360.0);
          if (angle_var.currentAngleZOffset < 0)
          {
               angle_var.currentAngleZOffset += 360.0;
          }
          currentEstimatedAngle = 0;
      }
      else{
        
      }
     
  }

  
  if (doSend) {
    doSend = false;
    updateGraph(dataAll,mpuTemperature,GyroZ,currentEstimatedAngle);
    //Serial.println(ESP.getFreeHeap());
  }
  */
    
  switch (currentState)
  {  
    /*case state_mapping:
            doc["time"] = CurrentTime;
            doc["value"] = angle_var.currentLunaDistance;//a random special num
            doc["units"] = "cm";
            doc["angle"] = currentEstimatedAngle;
            doc["speed"] = GyroZ;
            serializeJson(doc, general_output);
            if (!clients.empty()) {
              Serial.println("clients list not empty");
              //send data to the first client in the list
              sendDataToClient(clients[0], general_output);
            }
            if (abs(GyroZ) <= GYROZ_THRESHOLD_FOR_ROTATION_STOP)// 5rad/sec
            {  
              doc["time"] = CurrentTime;          
              doc["value"] = 77777;//a random special num
              doc["units"] = "ROTATION STOPPED!!!!!!!";
              serializeJson(doc, general_output);
              if (!clients.empty()) {
                Serial.println("clients list not empty");
                //send data to the first client in the list
                sendDataToClient(clients[0], general_output);
              }

              currentState = state_waiting_to_rotate;
            }
            else{
              currentState = state_mapping;
            }
            break;

    
    */
    case state_waiting_to_rotate:
        
      if (abs(GyroZ) >= GYROZ_THRESHOLD_FOR_ROTATION_START)
      {
        sendStartSpinningMassege();
        currentState = state_rotating;
        //move to next state according to configuration
        /*if(CONFIGURATION_MAPPING){
           currentState = state_mapping;
        }
        else{
           currentState = state_waiting_to_beacon;
        }
        */
      }
      else{
        currentState = state_waiting_to_rotate;
      }
      break;
    //first round/few rounds is not stable and gyro value also is not reliable 
    //because we manually rotate the device which causes shaking and ireliable values for speed 
    /*case state_waiting_to_beacon:
        
        //first time we see beacon we have a right offset for the angle that we can count on
        if(Beacon_ahead){
          previousTime = millis();//first time seeing beacon (we know there is at average speed of 3Hz for 10 sec)
          currentState = state_rotating;
        }
        else{
          currentState = state_waiting_to_beacon;
        }
        break;
    */
    case state_rotating:
        if (abs(GyroZ) <= GYROZ_THRESHOLD_FOR_ROTATION_STOP)// 5rad/sec
        {
            //TODO: put back after battery charge
            
            sendStopSpinningMassege();
            /*{
              doc["value"] = Counter_0_deg;
              doc["units"] = "times entered 0 deg";
              serializeJson(doc, general_output);
              if (!clients.empty()) {
                Serial.println("clients list not empty");
                //send data to the first client in the list
                sendDataToClient(clients[0], general_output);
              }
              doc["value"] = Counter_See_Beacon;
              doc["units"] = "times saw beacon";
              serializeJson(doc, general_output);
              if (!clients.empty()) {
                Serial.println("clients list not empty");
                //send data to the first client in the list
                sendDataToClient(clients[0], general_output);
              }
              doc["value"] = Counter_data_unreliable;
              doc["units"] = "unreliable data";
              serializeJson(doc, general_output);
              if (!clients.empty()) {
                Serial.println("clients list not empty");
                //send data to the first client in the list
                sendDataToClient(clients[0], general_output);
              }

              doc["value"] = avg_luna_distance;
              doc["units"] = "average distance";
              serializeJson(doc, general_output);
              if (!clients.empty()) {
                Serial.println("clients list not empty");
                //send data to the first client in the list
                sendDataToClient(clients[0], general_output);
              }
              
              digitalWrite(PIN1,LOW);
              digitalWrite(PIN2,LOW);
              digitalWrite(PIN3,LOW);
              digitalWrite(PIN4,LOW);
              digitalWrite(PIN5,LOW);
              digitalWrite(PIN6,LOW);
              digitalWrite(PIN7,LOW);
            }*/
            
            currentState = state_waiting_to_rotate;
            break;
        }

        /*{
          if((CurrentTime - previousTime) >= 1000){
              next_round = true;
          }
          if((CurrentTime_90deg - previousTime_90deg) >= 1000){
              next_round_90deg = true;
          }
          if((CurrentTime_neg90deg - previousTime_neg90deg) >= 1000){
              next_round_neg90deg = true;
          }
          if ((currentEstimatedAngle <= 10 && currentEstimatedAngle >= 0) && (next_round == true))
          {
              previousTime = millis();
              next_round = false;
              Counter_0_deg += 1; 
            if(DISPLAY_ARROW) {
                Time = Length/GyroZ;//total time for leds control 
                delay1 = abs((int)(round((Time)*pow(10,6))));//in microseconds
                display_arrow();
                currentState = state_rotating;
                break;
            }
          }
          
          if ((currentEstimatedAngle >= 265 && currentEstimatedAngle <= 275) && (next_round_90deg == true)){
              previousTime_90deg = millis();
              next_round_90deg = false;
              if(DISPLAY_ARROW) {
                Time = Length/GyroZ;//total time for leds control 
                delay1 = abs((int)(round((Time)*pow(10,6))));//in microseconds
                display_arrow();
                currentState = state_rotating;
                break;
            }
          }
          if ((currentEstimatedAngle >= 85 && currentEstimatedAngle <= 95) && (next_round_neg90deg == true)){
              previousTime_neg90deg = millis();
              next_round_neg90deg = false;
              if(DISPLAY_ARROW) {
                Time = Length/GyroZ;//total time for leds control 
                delay1 = abs((int)(round((Time)*pow(10,6))));//in microseconds
                display_arrow();
                currentState = state_rotating;
                break;
            }
          }
        }*/
        currentState = state_rotating;
        break;

        
    

  }

}
