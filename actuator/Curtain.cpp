/*
This code is developed as a part of
AUT IoT lab project for smart educational buildings.
Developed by Mohammad Hussein Tavakoli Bina
December 2016
All rights are reserved
*/
#include <arduino.h>
#include "Curtain.h"
#include <IRremote.h>
unsigned int ON[] = {9000, 4500, 560, 560, 560, 560, 560, 1690, 560, 560, 560, 560, 560, 560, 560, 560, 560, 560, 560, 1690, 560, 1690, 560, 560, 560, 1690, 560, 1690, 560, 1690, 560, 1690, 560, 1690, 560, 560, 560, 560, 560, 560, 560, 1690, 560, 560, 560, 560, 560, 560, 560, 560, 560, 1690, 560, 1690, 560, 1690, 560, 560, 560, 1690, 560, 1690, 560, 1690, 560, 1690, 560, 39416, 9000, 2210, 560}; //AnalysIR Batch Export (IRremote) - RAW

const int led_pin= 4;

Curtain::Curtain(int numberOfCurtains){
    _numberOfCurtains = numberOfCurtains;
    //define needed outputs
    switch (_numberOfCurtains) {
      case 1:
        pinMode(cw1_pin, OUTPUT);
        pinMode(ccw1_pin, OUTPUT);
        break;
      case 2:
        pinMode(cw1_pin, OUTPUT);
        pinMode(ccw1_pin, OUTPUT);
        pinMode(cw2_pin, OUTPUT);
        pinMode(ccw2_pin, OUTPUT);
        break;
      case 3:
        pinMode(cw1_pin, OUTPUT);
        pinMode(ccw1_pin, OUTPUT);
        pinMode(cw2_pin, OUTPUT);
        pinMode(ccw2_pin, OUTPUT);
        pinMode(cw3_pin, OUTPUT);
        pinMode(ccw3_pin, OUTPUT);
        break;
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
        break;
      case 2:
        digitalWrite(ccw2_pin, HIGH);
        delay(delayValue);
        digitalWrite(ccw2_pin, LOW);
        break;
      case 3:
        digitalWrite(ccw3_pin, HIGH);
        delay(delayValue);
        digitalWrite(ccw3_pin, LOW);
        break;
      case 4:
        digitalWrite(ccw1_pin, HIGH);
        digitalWrite(ccw2_pin, HIGH);
        digitalWrite(ccw3_pin, HIGH);
        delay(delayValue);
        digitalWrite(ccw1_pin, LOW);
        digitalWrite(ccw2_pin, LOW);
        digitalWrite(ccw3_pin, LOW);
        break;
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
        break;
      case 2:
        digitalWrite(cw2_pin, HIGH);
        delay(delayValue);
        digitalWrite(cw2_pin, LOW);
        break;
      case 3:
        digitalWrite(cw3_pin, HIGH);
        delay(delayValue);
        digitalWrite(cw3_pin, LOW);
        break;
      case 4: // Activate all curtains.
        digitalWrite(cw1_pin, HIGH);
        digitalWrite(cw2_pin, HIGH);
        digitalWrite(cw3_pin, HIGH);
        delay(delayValue);
        digitalWrite(cw1_pin, LOW);
        digitalWrite(cw2_pin, LOW);
        digitalWrite(cw3_pin, LOW);
        break;
    }

    digitalWrite(led_pin, LOW);
  }

bool Curtain::frame_is_mine(char frame[]){
    if(frame[0]=='@' && frame[1]=='3'&&  frame[3]=='p'){
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



//**********************************************************************************************
  actuator::actuator(int _start_pin,int _end_pin)
  {
    end_pin=_end_pin;
    start_pin=_start_pin;
    for(int i=_start_pin-1;i<_end_pin;i++)
    {
      pinMode(pins[i],OUTPUT);
      digitalWrite(pins[i],0);
    }
  }

 
  void actuator::handle_on_off(String frame)
  {
    
    int pin;
    int state;
    
    if(frame[0]=='@' && frame[1]==actuator_id &&  frame[3]=='l')
    {
      
      for(int i=0;i<frame.length();i++)
      {
        
        if(frame[i]==',' && frame[i+1]=='l')
        {
          
          pin=String(frame[i+2]).toInt();
          state=String(frame[i+3]).toInt();
          if(pin>=start_pin && pin<=end_pin)
          {
            digitalWrite(pins[pin-1],state);
          }
          
        }
      
      }
    }
  }


//*******************************************************************************************  
  ir::ir(int a)
  {
    int b=a;//kheeeeeeeeeeee
  }
  
   
  void ir::handle_ir(String frame)
  {
    IRsend irsend;
    
    if(frame[0]=='@' && frame[1]==actuator_id &&  frame[3]=='c')
    {
      
      for(int i=0;i<frame.length();i++)
      {
        String command="";
        int ctrl=-1;
        if(frame[i]==',' && frame[i+1]=='c')
        {
          for(int j=i+2;j<i+6 && frame[j]!=',' && frame[j]!='.';j++)
          {
            command=command+frame[j];
          }
          
          //Serial.print(command);
          ctrl=command.toInt();
          //Serial.print(ctrl);
          switch(ctrl)
          {
            case 1:
              Serial.println("ON");
              irsend.sendRaw(ON, sizeof(ON) / sizeof(ON[0]), khz);
              break;
    
            case 0:
              Serial.println("Off");
              //irsend.sendRaw(OFF, sizeof(OFF) / sizeof(OFF[0]), khz);
              break;
    
            case 17:
              //irsend.sendRaw(t17, sizeof(t17) / sizeof(t17[0]), khz);
              break;
           
            case 18:
              irsend.sendRaw(ON, sizeof(ON) / sizeof(ON[0]), khz);("T18");
              break;
            
            case 19:
              irsend.sendRaw(ON, sizeof(ON) / sizeof(ON[0]), khz);("T19");
              break;
            
            case 20:
              irsend.sendRaw(ON, sizeof(ON) / sizeof(ON[0]), khz);("T20");
              break;
            
            case 21:
              irsend.sendRaw(ON, sizeof(ON) / sizeof(ON[0]), khz);("T21");
              break;
            
            case 22:
              irsend.sendRaw(ON, sizeof(ON) / sizeof(ON[0]), khz);("T22");
              break;       
            
            case 23:
              irsend.sendRaw(ON, sizeof(ON) / sizeof(ON[0]), khz);("T23");
              break;

            case 24:
              irsend.sendRaw(ON, sizeof(ON) / sizeof(ON[0]), khz);("T24");
              break;
            
            case 25:
              irsend.sendRaw(ON, sizeof(ON) / sizeof(ON[0]), khz);("T25");
              break;
            
            case 26:
              irsend.sendRaw(ON, sizeof(ON) / sizeof(ON[0]), khz);("T26");
              break;

            case 27:
              irsend.sendRaw(ON, sizeof(ON) / sizeof(ON[0]), khz);("T27");
              break;
            
            case 28:
              irsend.sendRaw(ON, sizeof(ON) / sizeof(ON[0]), khz);("T28");
              break;
            
            case 29:
              irsend.sendRaw(ON, sizeof(ON) / sizeof(ON[0]), khz);("T29");
              break;
            
            case 30:
              irsend.sendRaw(ON, sizeof(ON) / sizeof(ON[0]), khz);("T30");
              break;
            
            case 31:
              irsend.sendRaw(ON, sizeof(ON) / sizeof(ON[0]), khz);("auto");
              break;
      
            case 32:
              irsend.sendRaw(ON, sizeof(ON) / sizeof(ON[0]), khz);("cool");
              break;
            
            case 33:
              irsend.sendRaw(ON, sizeof(ON) / sizeof(ON[0]), khz);("dry");
              break;
    
            case 34:
              irsend.sendRaw(ON, sizeof(ON) / sizeof(ON[0]), khz);("fan");
              break;
    
            case 35:
              irsend.sendRaw(ON, sizeof(ON) / sizeof(ON[0]), khz);("heat");
              break;
    
            case 36:
              irsend.sendRaw(ON, sizeof(ON) / sizeof(ON[0]), khz);("HIGH");
              break;
            
            case 37:
              irsend.sendRaw(ON, sizeof(ON) / sizeof(ON[0]), khz);("LOW");
              break;
            }  
 
        }
      
      }
    }
  }
  
  

