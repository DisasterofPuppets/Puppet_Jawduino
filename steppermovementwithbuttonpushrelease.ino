#include <Stepper.h>
//Nema with L298N driver


#define buttonpin 3
#define endstop 4
#define spr 200

Stepper mystepper(spr,5,6,7,8);

void setup(){

pinMode(buttonpin,INPUT_PULLUP);
pinMode(endstop,INPUT_PULLUP);
pinMode(6,OUTPUT);
pinMode(7,OUTPUT);
pinMode(8,OUTPUT);
pinMode(9,OUTPUT);

mystepper.setSpeed(60); //60rpm

Serial.begin(115200);
}

void loop(){



Serial.print("Button : ");
Serial.println(digitalRead(buttonpin));
Serial.print("Endstop : ");
Serial.println(digitalRead(endstop));


if (digitalRead(buttonpin) == 0){
    mystepper.step(1);
    }

if (digitalRead(buttonpin) == 1 && digitalRead(endstop) == 1){
    mystepper.step(-1);
    }

  if (digitalRead(buttonpin) == 1 && digitalRead(endstop) == 0){
    mystepper.step(0);
    }
  



 
}
