/***********************************************************
08-14-2016 Mike North This is a little proof of concept 
to make a servo move in sync with audio.
*******************/
#include <Servo.h>
#include "SoftRcPulseOut.h"

Servo rotation;


#define TEST_PIN 11 //pin 11 set to ground will kick off the servo sweep test
int audio_value = 0;
long lastMsg = 0;
long sleepWindow = 300000; //if 5 minutes go by with no signal, then put the servos to bed
SoftRcPulseOut servo;
volatile boolean servosEnabled = false;
volatile boolean ledsOn = true;
volatile unsigned long currentTime = 0;
volatile unsigned long lastLEDtime = 0;
unsigned long resetWait = 120000; //servos sleep if not changed within this time frame (120 secs)



void setup()
{

  Serial.begin(115200);
  set_minmax();
  pinMode(TEST_PIN,INPUT);  //pin 11 will be 
  digitalWrite(TEST_PIN,HIGH); //assign pull-up resistor
} 

void loop()
{
  servo_test();
  audio_updates(); //read sample from audio input and apply average to buffer
  if(servosEnabled) 
  {
  action(); //servos are handled in the action loop
  SoftRcPulseOut::refresh();
  if((millis() - lastMsg) > sleepWindow)
    detach_servos();
  }
} 
void attach_servos()
{   // attach the pin to the servo object
    servo.attach(2); // pin 2 for mouth
    rotation.attach(4); // Pin 4 for Head swivel servo
    servosEnabled = true;
}
void detach_servos()
{   // detach the servo objects
    servo.detach();
    servosEnabled = false;
}
void servo_test()
{
  if(digitalRead(TEST_PIN) == HIGH) return;
  attach_servos();
  SoftRcPulseOut::refresh();
  for(int i = 0; i < 360;i++)
  {
    if(i < 180)
      audio_value = i;
    else
      audio_value = 359 - i;
    action();
    for(int i = 0; i < 10; i++)
    {
       delay(1); 
       SoftRcPulseOut::refresh();
    }
  }
  detach_servos();
}
void audio_updates()
{
  audio_value = 0;
  if(analogRead(A0) < 341) audio_value += 60;
  if(analogRead(A1) < 341) audio_value += 60;
  if(analogRead(A2) < 341) audio_value += 60;

  if(audio_value > 0) 
  {
    lastMsg = millis(); //save the time stamp from when we last had some action
    if(!servosEnabled)attach_servos();
  }
}
void action() { 
  if (!servosEnabled) attach_servos();
  // set Neck servo back to 90 degress when audio comes in
  rotation.write(90);
  delay(15);
  servo.write(audio_value);
  SoftRcPulseOut::refresh();
 } 
void set_minmax()
{
  //set the first parameter in the following functions to a number between 0 and 180.
  //I used 92 and 72 in my tests to give about 20 degrees of motion.
  //You may swap the large and small numbers to reverse direction.
  //Just play with them, upload the code, then ground pin 11 to run the sweep test.
  // Be sure to only play with these numbers while the jaw linkage is disconnected,
  //  otherwise, you risk hitting mechanical limits and damaging your linkage or servo!
  servo.setMinimumPulse(map(0,0,180,512,2400));
  servo.setMaximumPulse(map(60,0,180,512,2400));
}
