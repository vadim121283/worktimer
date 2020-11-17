#include <Arduino.h>
#include <TM1637Display.h>

// Module connection pins (Digital Pins)
#define CLK 5
#define DIO 4

#define BTN_PIN 14

int seconds = 0;
int minutes = 0;
int hours = 0;
int dot = 1;
int timeInt = 0;
int tm = 0;
boolean startTimer = false;
uint8_t minusSeg[] = { 0x40, 0x40, 0x40, 0x40 };
boolean dispBr = true;

const uint8_t SEG_ANI1[] = { SEG_F, SEG_F, SEG_F, SEG_F };
const uint8_t SEG_ANI2[] = { SEG_A, SEG_A, SEG_A, SEG_A };
const uint8_t SEG_ANI3[] = { SEG_B, SEG_B, SEG_B, SEG_B };
const uint8_t SEG_ANI4[] = { SEG_C, SEG_C, SEG_C, SEG_C };
const uint8_t SEG_ANI5[] = { SEG_D, SEG_D, SEG_D, SEG_D };
const uint8_t SEG_ANI6[] = { SEG_E, SEG_E, SEG_E, SEG_E };

TM1637Display display(CLK, DIO);

#include "GyverButton.h"
GButton button(BTN_PIN, HIGH_PULL, NORM_OPEN);   // Button + GND - HIGH_PUL, Touch - LOW_PULL

void resetAnimation() {
  display.setSegments(SEG_ANI1);
  delay(100);
  display.setSegments(SEG_ANI2);
  delay(100);
  display.setSegments(SEG_ANI3);
  delay(100);
  display.setSegments(SEG_ANI4);
  delay(100);
  display.setSegments(SEG_ANI5);
  delay(100);
  display.setSegments(SEG_ANI6);
  delay(100);
}

void setTimeInt() {
  String h = String(hours);
  String timeString = "";
  String m = "0";
  String s = "0";
  if (minutes <= 9) {
    m = m + minutes;
  } else {
    m = String(minutes);
  }
  if (hours == 0) {
    if (seconds <= 9) {
      s = s + seconds;
    } else {
      s = String(seconds);
    }
    timeString = m + s;
    timeInt = timeString.toInt();
  } else {
    timeString = h + m;
    timeInt = timeString.toInt();
  }
}

void btnTick() {
  button.tick();
  //if (button.isClick()) {
  //  Serial.println("Click");
  //  startTimer = !startTimer;
  //}
  if (button.hasClicks()) {
    byte clicks = button.getClicks();
    switch (clicks) {
      case 1:
        if (startTimer) {
          startTimer = false;
          digitalWrite(2, HIGH);
        } else {
          startTimer = true;
          if (dispBr) {
            digitalWrite(2, LOW);
          }
        }
        break;
      case 2:
        dispBr = !dispBr;
        if (dispBr && startTimer) {
          digitalWrite(2, LOW);
        } else {
          digitalWrite(2, HIGH);
        }
        display.setBrightness(dispBr? 7 : 1);
        break;
      case 3:
        break;
      case 4:
        break;
      case 5:
        break;
    }
  }
  if (button.isHolded()) {
    // reset
    startTimer = false;
    digitalWrite(2, HIGH);
    hours = 0;
    minutes = 0;
    seconds = 0;
    setTimeInt();
    resetAnimation();
    display.setSegments(minusSeg);
  }
}

void tikSec() {
  if (tm == 0) {
    display.showNumberDecEx(timeInt, (0x80 >> dot), true);
    seconds++;
    if (dot == 0) {
      dot = 1;
    } else {
      dot = 0;
    }
    if (seconds >= 60) {
      seconds = 0;
      minutes++;
      if (minutes >= 60) {
        minutes = 0;
        hours++;
        if (hours >= 99) {
          hours = 0;
        }
      }
    }
    tm = 940;
    setTimeInt();
  }
  tm--;
  delay(1);
}

void setup() {
  display.clear();
  display.setBrightness(0x0f);
  display.setSegments(minusSeg);
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH); 
}

void loop() {
  btnTick();
  if (startTimer) {
    tikSec();
  }
}
