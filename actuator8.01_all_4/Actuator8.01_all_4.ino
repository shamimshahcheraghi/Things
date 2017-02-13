#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "printf.h"
#include "Curtain.h"
//#include "Timer1.h"

RF24 radio(9, 10);
Curtain myCurtain(1); //Construct curtain object for 1 curtain
actuator myactuator(3,6);
ir mycontrol(1);

const uint64_t pipes[2] = {0xE7D3F03577,0xC2C2C2C2C8};
String response_m="";
char charbuff[50];
const int led_pin = 4;
void ack(char text[]);

void ack(char index)
{
  //Serial.println("This is ack");
  char out[8]={0};
  char o[2]={0};
  o[0]=index;

  sprintf(out,"set%s",o);
  radio.openWritingPipe(pipes[1]);
  radio.stopListening();
  delayMicroseconds(200);

   radio.write(out,4);
}

void setup()
{
  while (!Serial);
  Serial.begin(9600);
  printf_begin();
  radio.begin();
  // 2 250 MicroSecond delays between each retry, 15 retries overally
  radio.setAutoAck(false);
  radio.setPALevel( RF24_PA_LOW ) ; //amplification level set at maximum
  radio.setChannel(85); //using first channel of frequency domain
  radio.setDataRate(RF24_250KBPS);// 250Kbps data rate
  //radio.setRetries(15,10);
  
  
  
  radio.printDetails();
  radio.openWritingPipe(pipes[0]);
  radio.stopListening();

  pinMode(led_pin, OUTPUT); //LED pin

  // send status notification at the startup
  radio.write("curtain1", 9);
  radio.write("curtain1", 9);
  radio.write("curtain1", 9);
  radio.write("curtain1", 9);

  digitalWrite(led_pin, HIGH);
  delay(250);
  digitalWrite(led_pin, LOW);
}
void loop()
{
  radio.startListening();
  radio.openReadingPipe(0, pipes[1]);
  
  if (radio.available())
  {
    
    ack(actuator_id); //acknowledgement sending
    char text[32] = {0};
    radio.read(&text, sizeof(text));
    Serial.println(text);
    myactuator.handle_on_off(String (text));
    mycontrol.handle_ir(String(text));
    
   
    if(myCurtain.frame_is_mine(text)){
      int value = myCurtain.extract_value(text);
      if(value>0){
          myCurtain.down(value,myCurtain.curtainID);
        }
      else{
          myCurtain.up(abs(value),myCurtain.curtainID);
        }
    }
  }
}

