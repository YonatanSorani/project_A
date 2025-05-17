#include "mpu.h"

Adafruit_MPU6050 mpu;

float angle = 0.0;


float ax_bias = 0, ay_bias = 0, az_bias = 0;
float gx_bias = 0, gy_bias = 0, gz_bias = 0;

void calibrateMPU(int samples = 20) {
  sensors_event_t a, g, temp;
  for (int i = 0; i < samples; i++) {
    mpu.getEvent(&a, &g, &temp);
    ax_bias += a.acceleration.x;
    ay_bias += a.acceleration.y;
    az_bias += a.acceleration.z - 9.81; // subtract gravity
    gx_bias += g.gyro.x;
    gy_bias += g.gyro.y;
    gz_bias += g.gyro.z;
    delay(5);
  }
  ax_bias /= samples;
  ay_bias /= samples;
  az_bias /= samples;
  gx_bias /= samples;
  gy_bias /= samples;
  gz_bias /= samples;

  
  Serial.println("Calibration complete:");
  Serial.print("Accel bias (m/s^2): x="); Serial.print(ax_bias, 3);
  Serial.print(", y="); Serial.print(ay_bias, 3);
  Serial.print(", z="); Serial.println(az_bias, 3);

  Serial.print("Gyro bias (rad/s): x="); Serial.print(gx_bias, 3);
  Serial.print(", y="); Serial.print(gy_bias, 3);
  Serial.print(", z="); Serial.println(gz_bias, 3);

}

void initMPU(){
  Wire.begin();       
  Wire.setClock(400000);        // Set I2C speed to 400 kHz

  // Initialize MPU
  if (!mpu.begin()) {
    delay(1);  // Optional, could be retry or error handling
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_2000_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_44_HZ);

  // Disable MPU interrupts (INT_ENABLE register = 0x38)
  Wire.beginTransmission(0x68); // Default MPU6050 address
  Wire.write(0x38);             // INT_ENABLE register
  Wire.write(0x00);             // Disable all interrupts
  Wire.endTransmission();

  //calibrateMPU();
}
  /*Serial.print(" Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println(" +-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println(" +-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println(" +-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println(" +-16G");
    break;
  }*/
  

  /*Serial.print(" Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    Serial.println(" +- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println(" +- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println(" +- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println(" +- 2000 deg/s");
    break;
  }
  */

  /*Serial.print(" Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    Serial.println(" 260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    Serial.println(" 184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    Serial.println(" 94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    Serial.println(" 44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    Serial.println(" 21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    Serial.println(" 10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    Serial.println(" 5 Hz");
    break;
  }

}*/


void readMPUValue(){
  sensors_event_t a, g, temp;
  mpu.getGyroSensor()->getEvent(&g);
  dataAll.GyroZ = (g.gyro.z-gz_bias); // Angular velocity in rad/s
  //dataAll.mpuTemperature = temp.temperature;
  
}

