# AIE

**중앙대학교 예술공학대학 2023학년도 2학기 피지컬 컴퓨팅 1분반 최종 프로젝트**

아두이노와 유니티 엔진을 활용한 게임 프로젝트

디지털 컨텐츠를 구현하여 하드웨어와 양방향 인터렉션이 가능한 인터렉티브 오브젝트 제작


# AIE_Project

Tools : Unity, Arduino

Unity Version : 2021.3.15f1

### Arduino Code

AIE_Project/Assets/Arduino/AIE_Arduino/AIE_Arduino.ino

### Unity Code

#### Singleton Pattern 및 아두이노 데이터 통신 관련

AIE_Project/Assets/Scripts/GameManager.cs

#### 유니티 큐브 회전 관련

AIE_Project/Assets/Scripts/CubeRotation.cs

#### 유니티 큐브 색상 변경 관련

AIE_Project/Assets/Scripts/CubeColorChange.cs

#### 게임 로직 관련

AIE_Project/Assets/Scripts/StartSceneManager.cs

AIE_Project/Assets/Scripts/PlaySceneManager.cs

### Unity Project

AIE_Project


# 최종 결과

### 구현 목록

#### 기기 모델링 완성

우레탄 보드 사용

스프레이 락카로 도색

#### 아두이노 센서 적용

로터리 엔코더 연결 성공

시프트 레지스터로 다수의 로터리 엔코더 연결 성공
 
네오픽셀 LED링 연결 성공

자이로센서 연결 성공

블루투스 모듈 연결 성공

보조배터리 연결 성공

#### 아두이노에서 유니티로의 데이터 전송

자이로 센서의 회전값을 유니티의 오브젝트에 적용 성공

로터리 엔코더의 회전을 읽어 유니티 큐브의 색상 변경 성공

#### 게임 메커니즘

아두이노 큐브의 회전값에 따라 유니티 큐브의 회전값 변경

아두이노 큐브의 다이얼을 돌려 유니티 큐브의 해당 면의 색상 변경

유니티 큐브의 각 면의 색상을 섞고, 플레이어는 주어진 시간동안 각 면의 색상을 암기하고, 이후 로터리 엔코더를 동작해 각 면의 색상을 변경해 색상을 일치하면 목표 달성



### 구현 실패 목록

#### 시프트 레지스터 고장

시프트 레지스터(74HC165) 납땜 도중 고장

여분 시프트 레지스터의 부재로 로터리 엔코더를 통한 큐브 색상 변화 기능 추가 실패

#### 유니티에서 아두이노의 데이터 전송 실패

유니티 큐브의 각 면 색상에 맞춰 LED링의 색상 변경 구현 실패