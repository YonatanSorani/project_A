#include "luna.h"

HardwareSerial LUNA_SERIAL(2);  // Use UART2

void initLuna() {
  LUNA_SERIAL.begin(115200, SERIAL_8N1, RXD2, TXD2);

  // Set TF-Luna output frequency to 20 Hz: command 5A 06 03 14
  uint8_t freqCmd[] = {0x5A, 0x06, 0x03, 0x1E};  // 0x14 = 20 // 0x1E =30
  LUNA_SERIAL.write(freqCmd, sizeof(freqCmd));
  uint8_t response[1];
  Serial2.readBytes(response, 1);
  while (response[0] != 0x5A) {
    Serial2.readBytes(response, 1);
  }

  if (response[0] == 0x5A) {
    uint8_t frequency_response[5];
    Serial2.readBytes(frequency_response, 5);
    Serial.println("Frequency set. Response:");
    for (int i = 0; i < 5; i++) {
      Serial.print(frequency_response[i], HEX);
      Serial.print(" ");
    }
    Serial.println();
  }
  
}

void readLidarValue() {
  static uint8_t buf[9];
  Serial.println(LUNA_SERIAL.available());
  while (LUNA_SERIAL.available() >= 9) {
    if (LUNA_SERIAL.read() == 0x59 && LUNA_SERIAL.peek() == 0x59) {
      LUNA_SERIAL.read();  // skip second 0x59
      LUNA_SERIAL.readBytes(buf, 7);  // Read next 7 bytes

      int dist = buf[0] + (buf[1] << 8);
      int strength = buf[2] + (buf[3] << 8);
      float temp = (buf[4] + (buf[5] << 8)) / 8.0 - 256;
       Serial.println("res");
      Serial.println(dist);
      Serial.println(strength);
      Serial.println(dist);
      Serial.println(temp);
      dataAll.distanceLuna = dist;
      dataAll.strengthLuna = strength;
      dataAll.temperatureLuna = temp;
    }
  }
}

