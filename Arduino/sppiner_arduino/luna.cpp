#include "luna.h"
bool got_data = false;
uint8_t buf[7] = {0}; // An array that holds TF-Luna data
uint8_t response[3] = {0};
uint8_t next_response[3] = {0};
int distanceLuna;
float temperatureLuna;

void readLidarValue(){
  //Read and print TF-Luna pins value
  
  while(!got_data){
    Serial2.readBytes(response, 1);
    while(response[0] != 0x59){
      Serial2.readBytes(response, 1);
    }
    Serial2.readBytes(next_response,1);
    if(next_response[0] == 0x59){
      got_data = true;
      Serial2.readBytes(buf,7);
      distanceLuna = buf[0] + buf[1] * 256;
      dataAll.strengthLuna = buf[2] + buf[3] * 256;
      temperatureLuna = buf[4] + buf[5] *256;//calculate chip temprature
      dataAll.temperatureLuna = temperatureLuna/8 - 256;
      if(!(dataAll.strengthLuna == (int)0xFFFF || dataAll.strengthLuna < (int)100)){
        dataAll.distanceLuna = distanceLuna;
      }
      else{
        dataAll.distanceLuna = -1; 
      }
    }
    else{
      got_data = false;
    }
  }
  got_data = false;
   
}


void initLuna(){
  //Initializing TF-Luna
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2); // Initializing serial port to read TF-Luna
  char freq_packet[] = {0x5A, 0x06, 0x03, 0x7D, 0x00, 0x00}; // set TF-Luna freq to 125 Hz (0x7D)
  Serial2.write(freq_packet, (size_t)6);
  uint8_t frequency_response[5];
  Serial2.readBytes(response, 1);
  while(response[0] != 0x5A){
    Serial2.readBytes(response, 1);
  }
  /*if(response[0] == 0x5A){
    Serial2.readBytes(frequency_response, 5);
    Serial.println("frequency is: ");
    Serial.println(response[0],HEX);
    Serial.println(frequency_response[0],HEX);
    Serial.println(frequency_response[1],HEX);
    Serial.println(frequency_response[2],HEX);
    Serial.println(frequency_response[3],HEX);    
    Serial.println(frequency_response[4],HEX);
  }  

  //get tf-luna version info
  char version_info[] = {0x5A, 0x04, 0x01, 0x00};
  Serial2.write(version_info,(size_t)4);
  uint8_t version_response[5];
  Serial2.readBytes(response, 1);
  while(response[0] != 0x5A){
    Serial2.readBytes(response, 1);
  }
  if(response[0] == 0x5A){
     Serial2.readBytes(version_response, 5);
     Serial.println("version is: ");
     Serial.println(response[0],HEX);
     Serial.println(version_response[0],HEX);
     Serial.println(version_response[1],HEX);
     Serial.println(version_response[2],HEX);
     Serial.println(version_response[3],HEX);    
     Serial.println(version_response[4],HEX);
 
  } */
}
