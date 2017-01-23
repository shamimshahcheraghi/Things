/*
This code is developed as a part of
AUT IoT lab project for smart educational buildings.
Developed by Mohammad Hussein Tavakoli Bina
December 2016
All rights are reserved
*/
#ifndef Curtain_h
#define Curtain_h

#include <arduino.h>

class Curtain{
  public:
    Curtain(int numberOfCurtains);
    void up(int value, int curtainID);
    void down(int value, int curtainID);
    bool frame_is_mine(char frame[]);
    int extract_value(char frame[]);
    int curtainID;
  private:
    //define pins statically
    //curtain 1 = left window
    const int cw1_pin = 5;
    const int ccw1_pin = 6;
    //curtain 2 = middle window
    const int cw2_pin = 7;
    const int ccw2_pin = 8;
    //curtain 3 = right window
    const int cw3_pin = A2;
    const int ccw3_pin = A3;
    //number of used curtains
    int _numberOfCurtains;
    const int _curtainSpeed = 10; //speed in cm/sec 
};

#endif
