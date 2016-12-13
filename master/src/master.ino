#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "printf.h"
#include <elapsedMillis.h>
#define interval 50

elapsedMillis stopwatch;
RF24 radio(9, 10);
int count=0;
boolean flag = false;
boolean ctr = false;
const uint64_t pipes[6] = { 0xE7D3F03577,
	0xC2C2C2C2C2,
	0xC2C2C2C2C3,
	0xC2C2C2C2C4,
	0xC2C2C2C2C5,
	0xC2C2C2C2C6
};   // Radio pipe addresses for the 6 nodes to communicate.
byte node_address=1;
void ack(char text[]);
String out_char="";
String c="";
char charbuff[50];
char charbuff1[50];
void setup()
{
	while (!Serial);
	Serial.begin(115200);
	printf_begin();
	radio.begin();
	radio.setPALevel( RF24_PA_MAX ) ;
	radio.setChannel(0);
	radio.setDataRate(RF24_250KBPS);
	radio.setRetries(2,15);
	radio.openReadingPipe(0, pipes[0]);
	radio.openReadingPipe(0, pipes[1]);
	radio.openReadingPipe(0, pipes[2]);
	radio.openReadingPipe(0, pipes[3]);
	radio.openReadingPipe(0, pipes[4]);
	radio.openReadingPipe(1, pipes[5]);
	radio.startListening();
	radio.printDetails();
}
void loop()
{
	radio.startListening();
	radio.openReadingPipe(0, pipes[0]);
	radio.openReadingPipe(1, pipes[5]);
	radio.openReadingPipe(1, pipes[4]);
	//Serial.println("hi");
	if (radio.available())
	{
		char text[33] = {0};
		radio.read(&text, 33);
		Serial.println(text);
		//Serial.println(count++);
		switch (text[1]) {
			case '1':
				radio.openWritingPipe(pipes[0]);
				radio.stopListening();
				delayMicroseconds(200);
				for(int i=0;i<2;i++){
					radio.write("ack1",sizeof("ack1"));
					delayMicroseconds(200);
				}

				break;
			case '2':
				radio.openWritingPipe(pipes[0]);
				radio.stopListening();
				delayMicroseconds(200);
				for(int i=0;i<2;i++){
					radio.write("ack2",sizeof("ack2"));
					delayMicroseconds(200);
				}

				break;
			case '3':
				radio.openWritingPipe(pipes[0]);
				radio.stopListening();
				delayMicroseconds(200);
				for(int i=0;i<2;i++){
					radio.write("ack3",sizeof("ack3"));
					delayMicroseconds(300);
				}

				break;
			case '4':
				radio.openWritingPipe(pipes[0]);
				radio.stopListening();
				delayMicroseconds(200);
				for(int i=0;i<2;i++){
					radio.write("ack4",sizeof("ack4"));
					delayMicroseconds(300);
				}

				break;
			case '5':
				radio.openWritingPipe(pipes[0]);
				radio.stopListening();
				delayMicroseconds(200);
				for(int i=0;i<2;i++){
					radio.write("ack5",sizeof("ack5"));
					delayMicroseconds(300);
				}

				break;
			case '6':
				radio.openWritingPipe(pipes[0]);
				radio.stopListening();
				delayMicroseconds(200);
				for(int i=0;i<2;i++){
					radio.write("ack6",sizeof("ack6"));
					delayMicroseconds(300);
				}

				break;
			case '7':
				radio.openWritingPipe(pipes[0]);
				radio.stopListening();
				delayMicroseconds(200);
				for(int i=0;i<2;i++){
					radio.write("ack7",sizeof("ack7"));
					delayMicroseconds(300);
				}

				break;
			case '8':
				radio.openWritingPipe(pipes[0]);
				radio.stopListening();
				delayMicroseconds(200);
				for(int i=0;i<2;i++){
					radio.write("ack8",sizeof("ack8"));
					delayMicroseconds(300);
				}

				break;
			case '9':
				radio.openWritingPipe(pipes[0]);
				radio.stopListening();
				delayMicroseconds(200);
				for(int i=0;i<2;i++){
					radio.write("ack9",sizeof("ack9"));
					delayMicroseconds(300);
				}

				break;

		}
	}
	int index1=0;
	char ch=0;

	if ( Serial.available() )
	{
		ch=0;
		c="";
		ch = Serial.read();
		c=c+ch;
		for(int i=0;i<32 && ch!='.' && (c[0]=='@' || c[0]=='g') ;i++)
		{
			while(!Serial.available());
			ch = Serial.read();

			c=c+ch;
			index1=i+2;//1 baraie in ke for az sefr mishomarad 1 baraie in ke ghabl az for yek char khande shode =>index=i+2;

		}
		if(c[0]=='@')
		{

			flag=false;
			c.toCharArray(charbuff1, index1+1);

			String s_buff="";
			s_buff=s_buff+'r'+charbuff1;
			Serial.println(s_buff);
			//Serial.println(index1);

			if(c[1]=='1')
			{
				radio.openWritingPipe(pipes[4]);
			}
			if(c[1]=='2')
			{
				radio.openWritingPipe(pipes[5]);
			}

			for(int i=0;i<6 && flag==false;i++)
			{
				delay(1);
				radio.stopListening();
				delay(1);
				radio.write(charbuff1,index1);
				if(c[1]=='1')
				{
					radio.openReadingPipe(0, pipes[4]);
				}
				if(c[1]=='2')
				{
					radio.openReadingPipe(0, pipes[5]);
				}
				radio.startListening();
				stopwatch=0; //stopwatch reset
				while(interval>stopwatch && flag==false)
				{
					if (radio.available())
					{
						char text[33] = {0};
						radio.read(&text, 33);
						//Serial.println("recived:");
						Serial.println(text);
						int result=1;
						if(charbuff1[1]=='1' )
						{
							result=strcmp(text, "set1");
							if(result==0)
							{

								flag=true;//out_char="";
								break;
							}
						}
						if(charbuff1[1]=='2' )
						{

							result=strcmp(text, "set2");
							if(result==0)
							{
								flag=true;//out_char="";
								break;
							}
						}
					}
				}

			}

		}
		else if(c[0]=='g')
		{

			ctr=false;
			c.toCharArray(charbuff1, index1+1);

			String s_buff="";
			s_buff=s_buff+'r'+charbuff1;
			Serial.println(s_buff);
			//Serial.println(index1);

			if(c[1]=='1')
			{
				radio.openWritingPipe(pipes[4]);
			}
			if(c[1]=='2')
			{
				radio.openWritingPipe(pipes[5]);
			}

			for(int i=0;i<6 && ctr==false;i++)
			{
				radio.stopListening();
				radio.write(charbuff1,index1);

				if(c[1]=='1')
				{
					radio.openReadingPipe(0, pipes[4]);
					radio.startListening();
				}
				if(c[1]=='2')
				{
					radio.openReadingPipe(0, pipes[5]);
					radio.startListening();
				}

				stopwatch=0; //stopwatch reset
				while((interval-30)>stopwatch && ctr==false)
				{
					if (radio.available())
					{
						char text[33] = {0};
						radio.read(&text, 33);
						Serial.println(text);
						if(text[0]=='g' && text[1]=='1'){
							ctr=true;
							break;
						}
						if(text[0]=='g' && text[1]=='2'){
							ctr=true;
							break;
						}
					}
				}

			}
		}

	}
}
