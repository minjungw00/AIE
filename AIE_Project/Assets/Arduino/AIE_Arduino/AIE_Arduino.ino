#include <SoftwareSerial.h>
#include <ShiftIn.h>
#include "Wire.h"

#define BT_RXD 6
#define BT_TXD 7

#define S_NUMBER 3
#define ARRAY_LENGTH 18

SoftwareSerial BTSerial(BT_RXD, BT_TXD);

// Init ShiftIn instance with one chip.
// The number in brackets defines the number of daisy-chained 74HC165 chips
// So if you are using two chips, you would write: ShiftIn<2> shift;
ShiftIn<S_NUMBER> shift;

int preEnc[ARRAY_LENGTH];
int curEnc[ARRAY_LENGTH];
int stateEnc[6] = {0, 0, 0, 0, 0, 0};

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

  // declare pins: pLoadPin, clockEnablePin, dataPin, clockPin
  shift.begin(2, 3, 4, 5);


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

void gyroConvertRawDataToAngle(int16_t AcX, int16_t AcY, int16_t AcZ) {
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
  BTSerial.print(",");
}

void gyroReadData(int8_t register_address) {

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
    gyroConvertRawDataToAngle(AcX, AcY, AcZ);
  }
}

// 현재 시프트 레지스터 값을 출력
void sendValues(){
  for(int i = 0; i < 6; ++i){
    BTSerial.print(stateEnc[i]);
    if(i == 5) BTSerial.print(";");
    else BTSerial.print(",");
  }
}

// 시프트 레지스터의 입력값이 변경됐는지 확인
void checkValues(){
  for(int i = 0; i < ARRAY_LENGTH; ++i){
    curEnc[i] = shift.state(i);
  }

  for(int i = 0; i < 6; ++i){
    if(preEnc[3 * i + 2] == 1){
      if(curEnc[3 * i + 2] == 0){
        stateEnc[i] = 3; // 클릭
        continue;
      }
    }
    if(preEnc[3 * i] == 1 && preEnc[3 * i + 1] == 1){
      if(curEnc[3 * i] == 1 && curEnc[3 * i + 1] == 1){
        stateEnc[i] = 0; // 정지
      }
      else if(curEnc[3 * i] == 0 && curEnc[3 * i + 1] == 1){
        stateEnc[i] = 1; // 반시계
      }
      else if(curEnc[3 * i] == 1 && curEnc[3 * i + 1] == 0){
        stateEnc[i] = 2; // 시계
      }
      else{
        stateEnc[i] = 4; // 회전중
      }
    }
    else{
      stateEnc[i] = 0; // 정지
    }
  }
}

// 입력값 갱신
void renewValues(){
  for(int i = 0; i < ARRAY_LENGTH; ++i){
    preEnc[i] = curEnc[i];
  }
}

void loop() {
  gyroReadData(0x3B);
  if(shift.update()){
    checkValues();
  }
  sendValues();
  renewValues();
  
  if(BTSerial.available()){
    Serial.write(BTSerial.read());
  }
  if(Serial.available()){
    BTSerial.write(Serial.read());
  }
  //delay(100);
}