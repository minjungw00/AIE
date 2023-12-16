#include <ShiftIn.h>

#define S_NUMBER 3
#define ARRAY_LENGTH 18

// Init ShiftIn instance with one chip.
// The number in brackets defines the number of daisy-chained 74HC165 chips
// So if you are using two chips, you would write: ShiftIn<2> shift;
ShiftIn<S_NUMBER> shift;

int preArr[ARRAY_LENGTH];
int curArr[ARRAY_LENGTH];

void setup() {
  Serial.begin(9600);

  // declare pins: pLoadPin, clockEnablePin, dataPin, clockPin
  shift.begin(2, 3, 4, 5);
}

// 현재 시프트 레지스터 값을 출력
void displayValues(){
  int n = 1;
  for(int i = 0; i < ARRAY_LENGTH; ++i){
    if(i % 3 == 0){
      Serial.print(n++);
      Serial.print("번 : ");
    }
    Serial.print(curArr[i]);
    if(i % 3 == 1){
      Serial.print(" ");
    }
    else if(i % 3 == 2){
      Serial.print("   ");
    }
  }
  Serial.println();
  
}

// 시프트 레지스터의 입력값이 변경됐는지 확인
bool checkValues(){
  for(int i = 0; i < ARRAY_LENGTH; ++i){
    curArr[i] = shift.state(i);
  }
  
  for(int i = 0; i < ARRAY_LENGTH; ++i){
    if(preArr[i] != curArr[i]) return true;
  }

  return false;
}

// 입력값 갱신
void renewValues(){
  for(int i = 0; i < ARRAY_LENGTH; ++i){
    preArr[i] = curArr[i];
  }
}

void loop() {
  if(shift.update()){
    if(checkValues()){
      displayValues();
    }
  }
  renewValues();
  delay(1);
}