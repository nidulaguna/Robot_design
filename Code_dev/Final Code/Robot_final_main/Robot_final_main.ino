#include <EasyNextionLibrary.h>
#include "distance.h"
EasyNex disp = EasyNex(Serial);

float speed = 62;
int level = 1;
float kp = 0.041;
float kd = 0.1;
bool measureDistance = false;


void setup() {
  disp.begin(9600);
  setID();
}

void loop() {
  disp.NextionListen();
  if (measureDistance == true) {
    disp.writeStr("t0.txt", (String)sensor_1());
  }
}


//Start button
void trigger0() {
  if (level == 1 || level == 3 || level == 4 || level == 5 || level == 6) {
    disp.writeStr("page task1");
    measureDistance = false;
  } else if (level == 2 || level == 7) {
    disp.writeStr("page task2");
    measureDistance = true;
  }
}

//level down
void trigger1() {
  level--;
  measureDistance = false;
  disp.writeStr("t0.txt", (String)level);
}

// level up
void trigger2() {
  level++;
  measureDistance = false;
  disp.writeStr("t0.txt", (String)level);
}

// speed down
void trigger3() {
  speed--;
  measureDistance = false;
  disp.writeStr("t2.txt", (String)speed);
}

// Speed Up
void trigger4() {
  speed++;
  measureDistance = false;
  disp.writeStr("t2.txt", (String)speed);
}

// calibrate button
void trigger5() {
  measureDistance = false;
}

// Home button
void trigger6() {
  speed++;
  measureDistance = false;
  disp.writeStr("page home");
  disp.writeStr("t0.txt", (String)level);
  disp.writeStr("t2.txt", (String)speed);
}

// goto calibrate menue
void trigger7() {
  measureDistance = false;
  disp.writeStr("page calib");
}

// Settings button
void trigger8() {
  measureDistance = false;
  disp.writeStr("page settings");
  disp.writeStr("t3.txt", (String)kd);
  disp.writeStr("t2.txt", (String)kp);
}

// Kp up
void trigger9() {
  kp += 0.001;
  measureDistance = false;
  disp.writeStr("t2.txt", (String)kp);
}

// Kp down
void trigger10() {
  kp -= 0.001;
  measureDistance = false;
  disp.writeStr("t2.txt", (String)kp);
}

// Kd up
void trigger11() {
  kd += 0.01;
  measureDistance = false;
  disp.writeStr("t3.txt", (String)kd);
}


// Kd down
void trigger12() {
  kd -= 0.01;
  measureDistance = false;
  disp.writeStr("t3.txt", (String)kd);
}

// Task stop button
void trigger13() {
measureDistance = false;
}