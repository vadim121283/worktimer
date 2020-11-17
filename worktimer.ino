#include <Arduino.h>
#include <TM1637Display.h>

// Module connection pins (Digital Pins) 5 4
#define CLK D1
#define DIO D2
//14
#define BTN_PIN D5

int seconds = 0;
int minutes = 0;
int hours = 0;
int dot = 1;
int timeInt = 0;
int tm = 0;
boolean startTimer = false;
uint8_t minusSeg[] = {0x40, 0x40, 0x40, 0x40};
boolean dispBr = true;
// 8 work hours countdown timer
int thours = 7;
int tminutes = 59;
boolean runCTimer = true;

//
//      A
//     ---
//  F |   | B
//     -G-
//  E |   | C
//     ---
//      D
const uint8_t SEG_ANIF[] = {SEG_F, SEG_F, SEG_F, SEG_F};
const uint8_t SEG_ANIA[] = {SEG_A, SEG_A, SEG_A, SEG_A};
const uint8_t SEG_ANIB[] = {SEG_B, SEG_B, SEG_B, SEG_B};
const uint8_t SEG_ANIC[] = {SEG_C, SEG_C, SEG_C, SEG_C};
const uint8_t SEG_ANID[] = {SEG_D, SEG_D, SEG_D, SEG_D};
const uint8_t SEG_ANIE[] = {SEG_E, SEG_E, SEG_E, SEG_E};

TM1637Display display(CLK, DIO);

#include "GyverButton.h"
GButton button(BTN_PIN, HIGH_PULL, NORM_OPEN); // Button + GND - HIGH_PUL, Touch - LOW_PULL

void resetAnimation()
{
  display.setSegments(SEG_ANIF);
  delay(100);
  display.setSegments(SEG_ANIA);
  delay(100);
  display.setSegments(SEG_ANIB);
  delay(100);
  display.setSegments(SEG_ANIC);
  delay(100);
  display.setSegments(SEG_ANID);
  delay(100);
  display.setSegments(SEG_ANIE);
  delay(100);
}

int getTimeInt(int hr, int mi, boolean cd)
{
  String h = String(hr);
  String timeString = "";
  int tInt = 0;
  String m = "0";
  String s = "0";
  if (mi <= 9)
  {
    m = m + mi;
  }
  else
  {
    m = String(mi);
  }
  if (cd)
  {
    timeString = h + m;
    tInt = timeString.toInt();
    tInt = tInt * (-1);
  }
  else
  {
    if (hr == 0)
    {
      if (seconds <= 9)
      {
        s = s + seconds;
      }
      else
      {
        s = String(seconds);
      }
      timeString = m + s;
      tInt = timeString.toInt();
    }
    else
    {
      timeString = h + m;
      tInt = timeString.toInt();
    }
  }
  return tInt;
}

void showCountdown()
{
  uint8_t data[] = {SEG_A | SEG_D, SEG_A | SEG_D, SEG_A | SEG_D, SEG_A | SEG_D};
  display.setSegments(data);
  delay(300);
  display.showNumberDecEx(getTimeInt(thours, tminutes, true), (0x80 >> (thours == 0 ? 0 : 1)), false);
  delay(2000);
  display.showNumberDecEx(getTimeInt(hours, minutes, false), (0x80 >> 1), true);
}

void btnTick()
{
  button.tick();
  if (button.hasClicks())
  {
    byte clicks = button.getClicks();
    switch (clicks)
    {
    case 1:
      if (startTimer)
      {
        startTimer = false;
        digitalWrite(2, HIGH);
      }
      else
      {
        startTimer = true;
        if (dispBr)
        {
          digitalWrite(2, LOW);
        }
      }
      break;
    case 2:
      // show countdown timer
      showCountdown();
      break;
    case 3:
      // reset countdown timer
      thours = 7;
      tminutes = 59;
      runCTimer = true;
      showCountdown();
      break;
    case 4:
      // low brightnes
      dispBr = !dispBr;
      if (dispBr && startTimer)
      {
        digitalWrite(2, LOW);
      }
      else
      {
        digitalWrite(2, HIGH);
      }
      display.setBrightness(dispBr ? 7 : 1);
      break;
    case 5:
      break;
    }
  }
  if (button.isHolded())
  {
    // reset
    startTimer = false;
    digitalWrite(2, HIGH);
    hours = 0;
    minutes = 0;
    seconds = 0;
    resetAnimation();
    display.setSegments(minusSeg);
  }
}

void tikSec()
{
  if (tm == 0)
  {
    display.showNumberDecEx(getTimeInt(hours, minutes, false), (0x80 >> dot), true);
    seconds++;
    if (dot == 0)
    {
      dot = 1;
    }
    else
    {
      dot = 0;
    }
    if (seconds >= 60)
    {
      seconds = 0;
      minutes++;
      if (minutes >= 60)
      {
        minutes = 0;
        hours++;
        if (hours >= 99)
        {
          hours = 0;
        }
      }
      if (runCTimer)
      {
        tminutes--;
        if (tminutes <= 0 && thours <= 0)
        {
          runCTimer = false;
        }
        else
        {
          if (tminutes < 0)
          {
            tminutes = 59;
            thours--;
          }
        }
      }
    }
    tm = 940;
  }
  tm--;
  delay(1);
}

void setup()
{
  display.clear();
  display.setBrightness(0x0f);
  display.setSegments(minusSeg);
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
}

void loop()
{
  btnTick();
  if (startTimer)
  {
    tikSec();
  }
}
