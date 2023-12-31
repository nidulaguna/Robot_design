#include <Arduino.h>
#include "line_following.h"
#include "BeeLineSensorPro.h"
#include "robot_arm.h"

//Initiating the sensor panel
BeeLineSensorPro sensor = BeeLineSensorPro((unsigned char[]){
                                             A0, A1, A2, A3, A4, A5, A6, A7 },
                                           LINE_WHITE);  // Need to change to LINE_WHITE

//define variable names for driver pins of motor 1 (right)
#define EN1 23
#define INA_1 4
#define INB_1 5

//define variable names for driver pins of motor 2 (left)
#define EN2 24
#define INA_2 6
#define INB_2 7

//define variable names for the junction detection sensors
#define rightSensor1 8
#define rightSensor2 9
#define leftSensor2 10
#define leftSensor1 13

// function declrarions
// void portFix();
// void rightmotor(float speed);
// void leftmotor(float speed);
// void leftTurn();
// void rightTurn();
// void calibrate();

//defining global variables
float last_error = 0;
float last_error_back = 0;
// int allBlackFlag = 0;

int z = 0;  // 1 for black line follow, 0 for white line follow

// void setup() {
//   portFix(); // Initializing the ports
//   calibrate(); // Calibrating the sensor panel

//   //delay is set to give time to place the robot in the starting square
//   delay(7000);
// }

// void loop() {

//   linefollow();

// }


// Initializing the I/O ports
void portFix() {

  // Initializing the Inputs of the line following sensor panel
  for (int i = A0; i <= A7; ++i) {
    pinMode(i, INPUT);
  }

  //Initializing the Outputs of Motor 1 (right)
  pinMode(EN1, OUTPUT);
  pinMode(INA_1, OUTPUT);
  pinMode(INB_1, OUTPUT);

  //Initializing the Outputs of Motor 2 (left)
  pinMode(EN2, OUTPUT);
  pinMode(INA_2, OUTPUT);
  pinMode(INB_2, OUTPUT);

  //Initializing the junction detection sensors
  pinMode(rightSensor1, INPUT);
  pinMode(rightSensor2, INPUT);
  pinMode(leftSensor1, INPUT);
  pinMode(leftSensor2, INPUT);
}

// Right motor working
void rightmotor(float speed) {
  digitalWrite(EN1, HIGH);
  if (speed > 0) {
    //Forward rotation
    digitalWrite(INB_1, HIGH);
    speed = speed;
  } else if (speed < 0) {
    //Backward rotation
    digitalWrite(INB_1, LOW);
    speed = -speed;
  }

  analogWrite(INA_1, speed);
}

// Left motor working
void leftmotor(float speed) {
  digitalWrite(EN2, HIGH);
  if (speed > 0) {
    //Forward rotation
    digitalWrite(INB_2, LOW);
    speed = speed;
  } else if (speed < 0) {
    //Backward rotation
    digitalWrite(INB_2, HIGH);
    speed = -speed;
  }

  analogWrite(INA_2, speed);
}

void rightTurn() {
  leftmotor(65);
  rightmotor(90);
  delay(300);
  leftmotor(0);
  rightmotor(0);
  delay(350);
  // sensor.readSensor();
  // for (int j = 0; j < 8; j++) {
  //   Serial.print((String)sensor.values[j] + " ");
  // }
  // Serial.println("");
  while (analogRead(A0) > 150 && analogRead(A1) > 150 && analogRead(A2) > 150 && analogRead(A3) > 150 && analogRead(A4) > 150 && analogRead(A5) > 150 && analogRead(A6) > 150){
    Serial.println("Inside the loop");
    leftmotor(-60);
    rightmotor(90);
    delay(2);
    // sensor.readSensor();
  }


  //delay(770);
  leftmotor(0);
  rightmotor(0);
  delay(350);

}

void leftTurn() {

  leftmotor(65);
  rightmotor(90);
  delay(300);
  leftmotor(0);
  rightmotor(0);
  delay(350);
  // sensor.readSensor();
  // for (int j = 0; j < 8; j++) {
  //   Serial.print((String)sensor.values[j] + " ");
  // }
  // Serial.println("");
  while (analogRead(A0) > 150 && analogRead(A1) > 150 && analogRead(A2) > 150 && analogRead(A3) > 150 && analogRead(A4) > 150 && analogRead(A5) > 150 && analogRead(A6) > 150){
    Serial.println("Inside the loop");
    leftmotor(60);
    rightmotor(-90);
    delay(2);
    // sensor.readSensor();
  }


  //delay(770);
  leftmotor(0);
  rightmotor(0);
  delay(350);
}

void calibrate() {
  //calibrating the sensor panel
  for (int j = 0; j < 4; j++) {
    int speedl = 65 * pow(-1, j);
    int speedr = 90 * pow(-1, j + 1);
    leftmotor(speedl);
    rightmotor(speedr);
    for (int i = 0; i < 150; i++) {
      sensor.calibrate();
      delay(5);
    }
  }
  rightmotor(0);
  leftmotor(0);
}

void linefollow(int baseSpeed_1,float Kp, float Kd) {
  //give sensor reading output as 1 for black color
  int left1 = digitalRead(leftSensor1);
  int left2 = digitalRead(leftSensor2);
  int right1 = digitalRead(rightSensor1);
  int right2 = digitalRead(rightSensor2);

  // if (allBlackFlag == 0) {  //defining the starting position
  //   rightmotor(90);
  //   leftmotor(65);
  //   delay(500);
  //   allBlackFlag++;
  // }
  // else if (allBlackFlag >=2){ //defining the ending position
  //   delay(500);
  //   rightmotor(0);
  //   leftmotor(0);
  // }
  if (right1 == z && right2 == z && left1 == z && left2 == z)  //detection of a straight line junction
  {
    // rightmotor(90);
    // leftmotor(65);
    delayMicroseconds(1);
    // allBlackFlag++;
  } else if ((left1 == z && left2 == z) && (right1 == 1 -z && right2 == 1 - z)) {  //detection of the 90 degree junction to the left
    rightmotor(90);
    leftmotor(65);
    delay(200);
    if (right1 == z && right2 == z && left1 == z && left2 == z)  // checking whether detected junction is a straight line junction
    {
      rightmotor(90);
      leftmotor(65);
      delay(100);
      // allBlackFlag++;
    } else {
      leftTurn();
    }
  } else if ((left1 == 1 - z && left2 == 1 - z) && (right1 == z && right2 == z)) {  //detection of the 90 degree junction to the right
    rightmotor(90);
    leftmotor(65);
    delay(200);
    if (right1 == z && right2 == z && left1 == z && left2 == z) {  //checking whether the detected junction is a straight line junction
      rightmotor(90);
      leftmotor(65);
      delay(100);
      // allBlackFlag++;
    } else {
      rightTurn();
    }
  } else {

    //line following is implemented
    // float Kp;
    // float Kd = 0.0252;  //line - 0.0252 wall - 0.02
    float err_avg = 0;

    for (int i = 0; i < 5; i++) {  //Average error is calculated
      err_avg += sensor.readSensor();


      delay(1);
    }
    err_avg = err_avg / 5;

    //Kp is set
    // Kp = 0.041;  // line - 0.041 wall - 0.015

    int baseSpeed = baseSpeed_1;  //Setting the base speed

    float pid = Kp * err_avg + Kd * (err_avg - last_error);  //Calculating the PID value

    last_error = err_avg;

    //Setting the speeds to the motor
    rightmotor(baseSpeed + pid + 20);
    leftmotor(baseSpeed - pid);

    delay(2);
  }
}

void backlinefollow(int baseSpeed_1,float Kp, float Kd) {

  // back line following is implemented
  // float Kp;
  // float Kd = 0.0252;  //line - 0.0252 wall - 0.02
  float err_avg = 0;

  for (int i = 0; i < 5; i++) {  //Average error is calculated
    err_avg += sensor.readSensor();


    delay(1);
  }
  err_avg = err_avg / 5;

  // Kp is set
  // Kp = 0.041;  // line - 0.041 wall - 0.015

  int baseSpeed = baseSpeed_1;  //Setting the base speed

  float pid = Kp * err_avg + Kd * (err_avg - last_error_back);  //Calculating the PID value

  pid = pid; // inverting the line following algorithm

  last_error_back = err_avg;

  //Setting the speeds to the motor
  rightmotor(-baseSpeed + pid - 20);
  leftmotor(-baseSpeed - pid);

  delay(2);
  }