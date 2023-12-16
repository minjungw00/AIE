#include <ShiftIn.h>

#define S_NUMBER 3
#define ARRAY_LENGTH 18

// Init ShiftIn instance with one chip.
// The number in brackets defines the number of daisy-chained 74HC165 chips
// So if you are using two chips, you would write: ShiftIn<2> shift;
ShiftIn<S_NUMBER> shift;

int preArr[ARRAY_LENGTH];
int curArr[ARRAY_LENGTH];
int stateArr[6] = {0, 0, 0, 0, 0, 0};

void setup() {
  Serial.begin(9600);

  // declare pins: pLoadPin, clockEnablePin, dataPin, clockPin
  shift.begin(2, 3, 4, 5);
}

// 현재 시프트 레지스터 값을 출력
void displayValues(){
  for(int i = 0; i < 6; ++i){
    if(stateArr[i] == 0){
      Serial.print("Stop ");
    }
    else if(stateArr[i] == 1){
      Serial.print("Anti ");
    }
    else if(stateArr[i] == 2){
      Serial.print("Clock ");
    }
    else if(stateArr[i] == 3){
      Serial.print("Click ");
    }
    else if(stateArr[i] == 4){
      Serial.print("Rot.. ");
    }
  }
  Serial.println();
}

// 시프트 레지스터의 입력값이 변경됐는지 확인
void checkValues(){
  for(int i = 0; i < ARRAY_LENGTH; ++i){
    curArr[i] = shift.state(i);
  }

  for(int i = 0; i < 6; ++i){
    if(preArr[3 * i + 2] == 1){
      if(curArr[3 * i + 2] == 0){
        stateArr[i] = 3; // 클릭
        continue;
      }
    }
    if(preArr[3 * i] == 1 && preArr[3 * i + 1] == 1){
      if(curArr[3 * i] == 1 && curArr[3 * i + 1] == 1){
        stateArr[i] = 0; // 정지
      }
      else if(curArr[3 * i] == 0 && curArr[3 * i + 1] == 1){
        stateArr[i] = 1; // 반시계
      }
      else if(curArr[3 * i] == 1 && curArr[3 * i + 1] == 0){
        stateArr[i] = 2; // 시계
      }
      else{
        stateArr[i] = 4; // 회전중
      }
    }
    else{
      stateArr[i] = 0; // 정지
    }
  }
}

// 입력값 갱신
void renewValues(){
  for(int i = 0; i < ARRAY_LENGTH; ++i){
    preArr[i] = curArr[i];
  }
}

void loop() {
  if(shift.update()){
    checkValues();
    displayValues();
  }
  renewValues();
  delay(1);
}