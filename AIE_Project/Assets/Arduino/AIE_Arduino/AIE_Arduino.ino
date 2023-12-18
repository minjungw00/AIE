#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>
#include <ShiftIn.h>
#include "Wire.h"


// 블루투스 관련 설정
#define BT_RXD 6
#define BT_TXD 7

SoftwareSerial BTSerial(BT_RXD, BT_TXD);
String receivedText;
char digits[6] = {0, 0, 0, 0, 0, 0};
int colors[6] = {0, 0, 0, 0, 0, 0};
int c = 0;


// 네오픽셀 LED링 관련 설정
//define NeoPixel Pin and Number of LEDs
#define LED_PIN 13
#define NUM_LEDS 96

#define S_NUMBER 3
#define ARRAY_LENGTH 18

//create a NeoPixel ring
Adafruit_NeoPixel ring = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);
int ledCount = 0;


// 시프트 레지스터 및 로터리 엔코더 관련 설정
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

// 자이로센서 데이터 각도를 Degree로 변환
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

// 자이로센서 데이터 읽기
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

// 시프트 레지스터 입력값 갱신
void renewValues(){
  for(int i = 0; i < ARRAY_LENGTH; ++i){
    preEnc[i] = curEnc[i];
  }
}

// 입력받은 색상 정보를 통해 LED링 색상 변경
// 미사용
void TurnOnLEDS(){
  ledCount = ++ledCount % 16;

  for(int i = 0; i < 6; ++i){
    for(int j = 0; j < 2; ++j){
      for(int k = 0; k < 8; ++k){
        if(digits[i] == 0){       // RED
          ring.setPixelColor(i * 16 + j * 8 + (ledCount + k) % 16, (255 / 8 * k), 0, 0);
        }
        else if(digits[i] == 1){  // ORANGE
          ring.setPixelColor(i * 16 + j * 8 + (ledCount + k) % 16, (255 / 8 * k), (127 / 8 * k), 0);
        }
        else if(digits[i] == 2){  // YELLOW
          ring.setPixelColor(i * 16 + j * 8 + (ledCount + k) % 16, (255 / 8 * k), (255 / 8 * k), 0);
        }
        else if(digits[i] == 3){  // GREEN
          ring.setPixelColor(i * 16 + j * 8 + (ledCount + k) % 16, 0, (255 / 8 * k), 0);
        }
        else if(digits[i] == 4){  // BLUE
          ring.setPixelColor(i * 16 + j * 8 + (ledCount + k) % 16, 0, 0, (255 / 8 * k));
        }
      }
    }
  }

  ring.show();
  delay(80);
}


void loop() {
  // 자이로센서 데이터 받기
  gyroReadData(0x3B);

  // 시프트 레지스터 입력 받기
  if(shift.update()){
    checkValues();
  }
  sendValues();
  renewValues();

  // 유니티에서 아두이노로 데이터 입력
  // 입력 데이터 로직 문제로 구현 비정상 작동
  // 유니티와 아두이노의 입력 신호 주기 차이가 문제일 것으로 추측
  if (BTSerial.available()) {
    for(int i = 0; i < 6; ++i){
      byte colorValue = BTSerial.read();
      Serial.println(colorValue);
    }
  }
  
  if(Serial.available()){
    BTSerial.write(Serial.read());
  }
}

void processReceivedData(String str) {
  Serial.println(str);
}