
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <LowPower.h>
#include <elapsedMillis.h>
#include <avr/wdt.h>
#include "DHT.h"
#define DHTPIN 5    
#define DHTTYPE DHT22   // DHT 22 (AM2302)
void mydelay(int x);
void read_sensor();
void TX(int t, int h, int l, int g, int w,int vbat);
void wakeUp();
DHT dht(DHTPIN, DHTTYPE);
RF24 radio(9,10);
boolean flag = false,motion=false;
const uint64_t pipes[1] = { 0xE7D3F03577}; 
const int light_pin = A2, Gass_pin = A0,wakeUpPin = 3;
int temp=26, humidity=18, te=0,hu=0;
int node_address=1;
String out_char="";
char charbuff[50];
int dataready=0;
String stringbuff = "";
int lightlevel=0, high = 0, low = 1023;
int interrupt_handled=0;
int index=0;
int sensorvalue;  // variable to store the value coming from the sensor
int vbat=0;
elapsedMillis stopwatch;

int readVcc() {
  // Read 1.1V reference against AVcc
  // set the reference to Vcc and the measurement to the internal 1.1V reference
  #if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
    ADMUX = _BV(MUX5) | _BV(MUX0);
  #elif defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
    ADMUX = _BV(MUX3) | _BV(MUX2);
  #else
    ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #endif  

  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Start conversion
  while (bit_is_set(ADCSRA,ADSC)); // measuring

  uint8_t low  = ADCL; // must read ADCL first - it then locks ADCH  
  uint8_t high = ADCH; // unlocks both

  long result = (high<<8) | low;

  result = 1125300L / result; // Calculate Vcc (in mV); 1125300 = 1.1*1023*1000
  return result; // Vcc in millivolts
}

void read_sensor(){
    te=dht.readTemperature();
    hu=dht.readHumidity();
  for(int i=1;te==0 && hu==0 && i<3 ;i++)
  {
    delay(1000+(i*500));
    te=dht.readTemperature();
    hu=dht.readHumidity();    
    wdt_reset();
  }
  if(te!=0 && hu!=0){
    temp=te;
    humidity=hu;
  }
  lightlevel = analogRead(light_pin);
  vbat=readVcc();
  //sensorvalue = analogRead(Gass_pin);
  TX(temp, humidity,lightlevel, sensorvalue, motion,vbat);
}
void TX(int t, int h, int l, int g, int m,int vbat)
{ 
  wdt_reset();
  radio.powerUp();
  digitalWrite(7, 1);
  out_char="";
  //sprintf(out_char,"@%d,t1%d,h1%d,l1%d,g1%d,m1%d,\n",node_address,t,h,l,g,m);
  out_char=out_char+'@'+node_address+",t1"+t+",h1"+h+",l1"+l+",m1"+m+","+vbat+".";
  for(int i=0;i<34;i++){
    if(out_char[i]=='.')
      index=i+2;
  }
  //Serial.println(index);
 
  flag=false;
  for(int i=0;i<10 && !flag;i++)
  {
      radio.openWritingPipe(pipes[0]);
      radio.stopListening();
      //Serial.println(out_char);
      out_char.toCharArray(charbuff, index);
      //Serial.println(charbuff);
      radio.write(charbuff,index); 
      radio.openReadingPipe(0, pipes[0]);
      radio.startListening();
      
    
     for(unsigned int j=0; j<2000  ;j++)
     {
      
      if (radio.available())
      {
        char text[35] = {0};
        radio.read(&text, sizeof(text));
        Serial.println(text);
        String ack="";
        ack=ack+"ack"+node_address;
        char const_ack[4]="";
        ack.toCharArray(const_ack, 5);
        int result=strcmp(text, const_ack);
        if(result==0)
        {
          motion=false;
          flag=true;
          out_char="";
          break;
        }    
      } 
    }
    
    //delay(100);
    
    
  }
    
 digitalWrite(4, 0);
 digitalWrite(7, 0);
}
void wakeUp()
{
    detachInterrupt(digitalPinToInterrupt(wakeUpPin));
    interrupt_handled=1;
    motion=true;
    radio.powerUp();
    digitalWrite(4, 1);
    digitalWrite(7, 1);
    lightlevel = analogRead(light_pin);
    //Serial.println("motion detection");
    TX(temp, humidity, lightlevel, sensorvalue, motion,vbat);
    digitalWrite(4, 0);
    digitalWrite(7, 0);   
}
void setup()
{             
    wdt_disable();
    //To reduce power, setup all pins as inputs with no pullups
    for(int x = 1 ; x < 18 ; x++)
    {
      pinMode(x, INPUT);
     // digitalWrite(x, LOW);
    }
    while (!Serial);
    Serial.begin(9600);
    radio.begin();
    radio.setPALevel( RF24_PA_MAX ) ;
    radio.setChannel(0);
    radio.setDataRate(RF24_250KBPS);
    radio.setRetries(2,15);
    radio.openWritingPipe(pipes[0]);
    radio.stopListening();
    
    // Configure wake up pin as input.
    // This will consumes few uA of current.
    pinMode(wakeUpPin, INPUT);   
    pinMode(5,OUTPUT); 
    pinMode(4,OUTPUT);
    pinMode(7,OUTPUT);
    digitalWrite(4,1);
    digitalWrite(7,1);
    delay(100);
    digitalWrite(7,0);
    delay(100);
    digitalWrite(7,1);
    delay(100);
    digitalWrite(7,0);
    String buff="";
    buff=buff+"node "+ node_address+" test";
    char out_buff[20];
    buff.toCharArray(out_buff, 12);
    radio.write(out_buff,13); 
    radio.write(out_buff,13); 
    radio.write(out_buff,13); 
    dht.begin();
    delay(700);

    //sensorvalue = analogRead(Gass_pin);
    wdt_enable(WDTO_8S);
    wdt_reset();
    read_sensor();
    //TX(temp, humidity,lightlevel, sensorvalue, motion,vbat);
    attachInterrupt(digitalPinToInterrupt(wakeUpPin), wakeUp, RISING);
}
void loop()
{        
      //delay(100);
      radio.powerDown(); 
      for(int i=0;i<10;i++)
      {
        wdt_disable();
        //delay(100);
        attachInterrupt(digitalPinToInterrupt(wakeUpPin), wakeUp, RISING);
        LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
        wdt_enable(WDTO_8S);
        wdt_reset();
      }
    if(interrupt_handled==0){
      radio.powerUp();
      digitalWrite(4, 1);
      //digitalWrite(7, 1);
      dht.begin();
      delay(700);
      read_sensor();
    }
    interrupt_handled=0;
}

