#Curtain Actuator
This is the source files for the curtain actuator. The board gives value for going up and down in centimeters(by means of NRF24L01) and does so.

#Contributers
* [Mohammad Hossein Tavakoli Bina](https://github.com/mhtb32)
* Sepehr Hashtroudi -> some debugging works.

#Command frame
The general form of command you should sent to curtain actuator is like this: `@3,pxsvvv`
Which these symbols stand for:

Symbol | meaning
------------ | -------------
@ | just for starting the frame
3 | id for actuator board
, | just for seperation
p | a constant symbol
x | ID for curtain you want to move
s | sign for amount of movement, can be **+** or **-**
vvv | three digit number for amount of movement, in range (-200,200)
