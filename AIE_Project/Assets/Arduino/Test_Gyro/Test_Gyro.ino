#include <SoftwareSerial.h>
#include "Wire.h"

#define BT_RXD 6
#define BT_TXD 7
SoftwareSerial BTSerial(BT_RXD, BT_TXD);

int8_t mpu_address = 0x68;
int sendCommand(int8_t register_address, int8_t register_value) {
  Wire.beginTransmission(mpu_address);
  Wire.write(register_address);
  Wire.write(register_value);
  int statusCommand = Wire.endTransmission(true);
  // Serial.print("I2C Address: "); Serial.print(mpu_address, HEX);
  // Serial.print(" Register address: "); Serial.print(register_address, HEX);
  // Serial.print(" Register value: "); Serial.print(register_value, HEX);
  // Serial.print(" Status command: "); Serial.println(statusCommand);

  return statusCommand;
}

void setup() {
  //join the I2C bus
  Wire.begin();
  //start serial communication
  Serial.begin(9600);
  BTSerial.begin(9600);
  delay(1000);

  /* sendCommand can return 5 possile values:
   * -> 0:success
   * -> 1:data too long to fit in transmit buffer
   * -> 2:received NACK on transmit of address
   * -> 3:received NACK on transmit of data
   * -> 4:other error
   */
  while(sendCommand(0x6B,0x0) != 0){
    delay(1000);
  }

  // Serial.println("MPU6050 started");
}

void convertRawDataToAngle(int16_t AcX, int16_t AcY, int16_t AcZ) {
  int minVal = 265; int maxVal = 402;
  int xAng = map(AcX, minVal, maxVal, -90, 90);
  int yAng = map(AcY, minVal, maxVal, -90, 90);
  int zAng = map(AcZ, minVal, maxVal, -90, 90);

  int x = RAD_TO_DEG * (atan2(-yAng, -zAng)+PI);
  int y = RAD_TO_DEG * (atan2(-xAng, -zAng) + PI);
  int z = RAD_TO_DEG * (atan2(-yAng, -xAng)+PI);

  //Serial.print("X angle: "); Serial.print(x);
  //Serial.print(" Y angle: "); Serial.print(y);
  //Serial.print(" Z angle: "); Serial.print(z);
  BTSerial.print(x);
  BTSerial.print(",");
  BTSerial.print(y);
  BTSerial.print(",");
  BTSerial.print(z);
  BTSerial.print(";");
}

void readData(int8_t register_address) {

  int bytesReceived = -1;
  int statusCommand = -1;
  do {
    Wire.beginTransmission(mpu_address);
    Wire.write(register_address);
    statusCommand = Wire.endTransmission(false);
    bytesReceived = Wire.requestFrom(mpu_address, 6, true);
    // Serial.print("I2C Address: "); Serial.print(mpu_address, HEX);
    // Serial.print(" Register address: "); Serial.print(register_address, HEX);
    // Serial.print(" Status command: "); Serial.print(statusCommand);
    // Serial.print(" Bytes received: "); Serial.println(bytesReceived);
  } while (statusCommand != 0 && bytesReceived != 6);
  //check if MPU6050 sent all the requested data
  //in total 6 bytes, 2 bytes for each axis
  if (Wire.available() == 6) {
    int16_t AcX = Wire.read() << 8 ;
    AcX |= Wire.read();
    int16_t AcY = Wire.read() << 8 ;
    AcY |= Wire.read();
    int16_t AcZ = Wire.read() << 8 ;
    AcZ |= Wire.read();
    convertRawDataToAngle(AcX, AcY, AcZ);
  }
}
void loop() {
  //put your main code here, to run repeatedly:
  readData(0x3B);
  if(BTSerial.available()){
    Serial.write(BTSerial.read());
  }
  if(Serial.available()){
    BTSerial.write(Serial.read());
  }
  //delay(100);
}