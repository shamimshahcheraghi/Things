/*
This code is developed as a part of
AUT IoT lab project for smart educational buildings.
Developed by Mohammad Hussein Tavakoli Bina
December 2016
All rights are reserved
*/
#include <arduino.h>
#include "Curtain.h"
const int led_pin= 4;

Curtain::Curtain(int numberOfCurtains){
    _numberOfCurtains = numberOfCurtains;
    //define needed outputs
    switch (_numberOfCurtains) {
      case 1:
        pinMode(cw1_pin, OUTPUT);
        pinMode(ccw1_pin, OUTPUT);
      case 2:
        pinMode(cw1_pin, OUTPUT);
        pinMode(ccw1_pin, OUTPUT);
        pinMode(cw2_pin, OUTPUT);
        pinMode(ccw2_pin, OUTPUT);
      case 3:
        pinMode(cw1_pin, OUTPUT);
        pinMode(ccw1_pin, OUTPUT);
        pinMode(cw2_pin, OUTPUT);
        pinMode(ccw2_pin, OUTPUT);
        pinMode(cw3_pin, OUTPUT);
        pinMode(ccw3_pin, OUTPUT);
    }
  }

void Curtain::up(int value,int curtainID){
    //Calculating delay value in milliSeconds
    digitalWrite(led_pin, HIGH);
    int delayValue = (value/_curtainSpeed)*1000;
    switch (curtainID) {
      case 1:
        digitalWrite(ccw1_pin, HIGH);
        delay(delayValue);
        digitalWrite(ccw1_pin, LOW);
      case 2:
        digitalWrite(ccw2_pin, HIGH);
        delay(delayValue);
        digitalWrite(ccw2_pin, LOW);
      case 3:
        digitalWrite(ccw3_pin, HIGH);
        delay(delayValue);
        digitalWrite(ccw3_pin, LOW);
      case 4:
        digitalWrite(ccw1_pin, HIGH);
        digitalWrite(ccw2_pin, HIGH);
        digitalWrite(ccw3_pin, HIGH);
        delay(delayValue);
        digitalWrite(ccw1_pin, LOW);
        digitalWrite(ccw2_pin, LOW);
        digitalWrite(ccw3_pin, LOW);
    }

    digitalWrite(led_pin, LOW);
  }

void Curtain::down(int value,int curtainID){
    //Calculating delay value in milliSeconds
    digitalWrite(led_pin, HIGH);
    int delayValue = (value/_curtainSpeed)*1000;
    switch (curtainID) {
      case 1:
        digitalWrite(cw1_pin, HIGH);
        delay(delayValue);
        digitalWrite(cw1_pin, LOW);
      case 2:
        digitalWrite(cw2_pin, HIGH);
        delay(delayValue);
        digitalWrite(cw2_pin, LOW);
      case 3:
        digitalWrite(cw3_pin, HIGH);
        delay(delayValue);
        digitalWrite(cw3_pin, LOW);
      case 4: // Activate all curtains.
        digitalWrite(cw1_pin, HIGH);
        digitalWrite(cw2_pin, HIGH);
        digitalWrite(cw3_pin, HIGH);
        delay(delayValue);
        digitalWrite(cw1_pin, LOW);
        digitalWrite(cw2_pin, LOW);
        digitalWrite(cw3_pin, LOW);
    }

    digitalWrite(led_pin, LOW);
  }

bool Curtain::frame_is_mine(char frame[]){
    if(frame[0]=='@' && frame[1]=='3'){
        return true;
      }
    else{
        return false;
      }
  }

int Curtain::extract_value(char frame[]){
    curtainID = String(frame[4]).toInt();
    
    char char_value[4] = "" ;
    char_value[0] = frame[5];
    char_value[1] = frame[6];
    char_value[2] = frame[7];
    char_value[3] = frame[8];

    //value for going up and down in cm
    int value = String(char_value).toInt();

    return value;
  }

