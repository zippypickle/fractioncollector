/**
2018.05.24
If you see this code and want to make the program work, hit the little magnifying
glass in the upper right corner of this window. Make sure it says 9600 baud.

This code works as an XY plotter for fraction collection.
The delay value is the amount of time for each fraction.
Otherwise the main functions are printed in the serial window
at startup. Additional functions are 0=xyhome only. More functions
can be added by assigning serial inputs as various programs.
 */

#include "MeOrion.h"
#include <SoftwareSerial.h>

MeStepper stepper(PORT_1); 

long Delay = 270000;  //1500 for demo, 270000 for 4:30.

int xy1 = 13;  //declare variables for limit switches
int xy2 = 2;
int xy3 = 12;
int xy4 = 8;

int dirPin = mePort[PORT_1].s1;  //the direction pin connect to Base Board PORT1 SLOT1
int stpPin = mePort[PORT_1].s2;  //the Step pin connect to Base Board PORT1 SLOT2

int dirPin2 = mePort[PORT_2].s1;  //the direction pin connect to Base Board PORT1 SLOT1
int stpPin2 = mePort[PORT_2].s2;  //the Step pin connect to Base Board PORT1 SLOT2

int xysensor1; //set some variables to store the limit switch status
int xysensor2;
int xysensor3;
int xysensor4;

void setup()  //do these things only once at startup
{
    Serial.begin(9600);
    pinMode(xy1, INPUT);  //set limit switches as inputs
    pinMode(xy2, INPUT);
    pinMode(xy3, INPUT);
    pinMode(xy4, INPUT);
        
    digitalWrite(xy1, HIGH); //set limit switches as 1 st start
    digitalWrite(xy2, HIGH);
    digitalWrite(xy3, HIGH);
    digitalWrite(xy4, HIGH);

  pinMode(16, OUTPUT);  //set the Analog2 as a digital output
  digitalWrite(16, HIGH); //set A2 as high or off to start
  pinMode(14, OUTPUT);
  digitalWrite(14, HIGH);

  pinMode(dirPin, OUTPUT); //setup for the stepper motor1
  pinMode(stpPin, OUTPUT);
  
  pinMode(dirPin2, OUTPUT); //setup for the stepper motor 2
  pinMode(stpPin2, OUTPUT);

  stepper.setMaxSpeed(1000);
  stepper.setAcceleration(20000);

  //Now print out some instructions for the serial interface

 Serial.println("Welcome to the Ghetto Fabuluous VBIO Fraction Collector!");
 Serial.println(" ");
 Serial.println("Press 1 or 2 then enter to walk the collection head left/right 160 steps.");
 Serial.println("Press 3 or 4 then enter to walk the collection head front/back 160 steps.");
 Serial.println("Press c/v or z/x for smaller stepping L/R, For/Rev directions.");
 Serial.println("Press 6 then enter to open pinch valve, 5 to close it.");
 Serial.println("Press w then enter to turn the pump on, or q to shut it off.");
 Serial.println("Press h/j/k/l/t then enter to XYhome then start a 1/2/3/4/5 rack run, respectively.");
 Serial.println("Press 9 if you want to start a single vertically oriented rack from the current starting point");
  
}

void step(boolean dir,int steps)  //code to make the stepper motors work - don't change
{
  digitalWrite(dirPin,dir);
  delay(50);
  for(int i=0;i<steps;i++)
  {
    digitalWrite(stpPin, HIGH);
    delayMicroseconds(800);
    digitalWrite(stpPin, LOW);
    delayMicroseconds(800); 
  }
}
void step2(boolean dir,int steps) //code to make the stepper motors work - don't change
{
  digitalWrite(dirPin2,dir);
  delay(50);
  for(int i=0;i<steps;i++)
  {
    digitalWrite(stpPin2, HIGH);
    delayMicroseconds(800);
    digitalWrite(stpPin2, LOW);
    delayMicroseconds(800); 
  }
}



void loop()  //this code repeats every cycle
{
//  xysensor1 = digitalRead(xy1);  //I think I had this coded during troubleshooting. Not sure if necessary anymore
//  xysensor2 = digitalRead(xy2);
//  xysensor3 = digitalRead(xy3);
 // xysensor4 = digitalRead(xy4);
  
  if(Serial.available())
  {
    char a = Serial.read();
    switch(a)
    {
      case '0':
      xyhome();
      break;
      case '1':
      step2(1,185);  // go X + (homeward!!!)
      break;
      case '2':
      step2(0,185);  // go X- (AWAY)
      break;
      case '3':
      step(0,160);  // go Y- (homeward!!!)
      break;
      case '4':
      step(1,160);  // go Y+ (AWAY)
      break;
      case '5':
  //    step2(1,20);  // go X + (homeward!!!)
         digitalWrite(16, HIGH); //turn off flow
      break;
      case '6':
  //    step2(0,20);  // go X- (AWAY)
         digitalWrite(16, LOW);  //turn on flow
      break;
            case 'q':
  //    step2(1,20);  // go X + (homeward!!!)
         digitalWrite(14, HIGH); //turn off flow
      break;
      case 'w':
  //    step2(0,20);  // go X- (AWAY)
         digitalWrite(14, LOW);  //turn on flow
      break;
      case 'z':
      step(0,20);  // go down/forward small steps
      break;
      case 'x':
      step(1,20);  // go up/away small steps
      break;
      case 'c':
      step2(1,20);  // go left small steps
      break;
      case 'v':
      step2(0,20);  // go right small steps
      break;
      case '9':
      rack1();
      break;
            case 'h':
      masterRack1();
      break;
            case 'j':
      masterRack2();
      break;
            case 'k':
      masterRack3();
      break;
            case 'l':
      masterRack4();
      break;
      case 'p':
      xyhome3();
      break;
      case 'y':
      rack5();
      break;
      case 't':
      WholeEnchilada();
      break;

      

      //there can be infinite commands from the serial input. these are just the ones that seemed helpful.
    }
  }
  stepper.run();  //after making a selection and hitting enter, this makes it happen.

}

void xyhome()  //homing function - press 0 at the serial and it will walk to the home.
{
  int xysensor3 = digitalRead(xy3);
  int xysensor2 = digitalRead(xy2);

  while(xysensor3 == 1) {  //if not at X=0, take baby steps until you get there
    step2(1,2);
    xysensor3 = digitalRead(xy3);
}
  while(xysensor3 == 0 && xysensor2 == 1) {  //while at X=0 but not Y=0, step in the Y direction
    step(0,2);
    xysensor3 = digitalRead(xy3);
    xysensor2 = digitalRead(xy2);
    
}
}

void yhome()   //experimental code to only zero on the Y plane. might be useful to re-establish only Y.
{

  int xysensor2 = digitalRead(xy2);

  while(xysensor2 == 1) {
    step(0,2);
    xysensor2 = digitalRead(xy2);
}

}


  void xyhome2()  //to align the carriage with the motor corner - upper right. not in use yet.
{
  int xysensor1 = digitalRead(xy1);
  int xysensor4 = digitalRead(xy4);

  while(xysensor1 == 1) {
    step2(0,2);
    xysensor1 = digitalRead(xy1);
}
  while(xysensor1 == 0 && xysensor4 == 1) {
    step(1,2);
    xysensor1 = digitalRead(xy1);
    xysensor4 = digitalRead(xy4);
    
}
}

  void xyhome3()  //to align the carriage with the upper left corner- for the fifth rack
{
  int xysensor3 = digitalRead(xy3);
  int xysensor4 = digitalRead(xy4);

  while(xysensor3 == 1) {
    step2(1,2);
    xysensor3 = digitalRead(xy3);
}
  while(xysensor3 == 0 && xysensor4 == 1) {
    step(1,2);
    xysensor3 = digitalRead(xy3);
    xysensor4 = digitalRead(xy4);
    
}
}


  void rack1()  //this is the main program to start wherever it is positioned and walk through a red rack.
{
    digitalWrite(16, LOW); //open pinch valve
    digitalWrite(14, LOW); //turn on pump
    delay(Delay);
  for (int index = 0; index < 7; index++) {
    step(1,160);
    delay(Delay);
    }
  
  digitalWrite(16, HIGH); //close pinch valve
  delay(100);
  digitalWrite(14, HIGH); //turn off pump
  delay(100);
  step2(0,90);
  delay(50);
  step(0,1125);
  delay(50);
  step2(0,95);
    digitalWrite(16, LOW); //open pinch valve
    delay(50);
    digitalWrite(14, LOW); //turn on pump
  delay(Delay);  //need this delay for the first tube to fill, before entering the for loop
  
  for (int index = 0; index < 7; index++) {
    step(1,160);
    delay(Delay);
    }
 
  digitalWrite(16, HIGH); //close pinch valve
  delay(100);
  digitalWrite(14, HIGH); //turn off pump
  delay(100);
  step2(0,90);
  delay(50);
  step(0,1125);
  delay(50);
  step2(0,95);
    digitalWrite(16, LOW); //open pinch valve
    digitalWrite(14, LOW); //turn on pump
  delay(Delay);
    
  for (int index = 0; index < 7; index++) {
    step(1,160);
    delay(Delay);
    }
  digitalWrite(16, HIGH); //close pinch valve
  delay(100);
  digitalWrite(14, HIGH); //turn off pump
  delay(100);
    
  }
    

void masterRack4()  //this is the main program from entering 'run4'. Home, then 4 racks.
{
  xyhome();  // zero out in the corner
  step(1,20);  //step 40x in the X direction from home
  step2(0,50); //step 40x in the Y direction from home
      
  rack1();  //run rack position 1
  
  step2(0,118); //move in the X direction to the right - half way
  step(0,1100);  //move back to the front tubes - Y direction
  step2(0,118); //move in the X direction the rest of the way to the right


  rack1();  //run rack position 2
  
  step2(0,115); //changed from 118 on 20180525
  step(0,1100);
  step2(0,115);


  rack1();  //run rack position 3

  step2(0,115);  //this was already 115 to prevent going too far to the right.
  step(0,1100);
  step2(0,115);


  rack1();  //run rack position 4
  digitalWrite(16, LOW); //open pinch valve to relieve pressure
  delay(5000);  //give it 5 seconds
  digitalWrite(16, HIGH); //close pinch valve
  //this program stops after the 4th rack.

}

void masterRack2()  //this is the program linked to 'run2'. Home, then 2 racks.
{
  xyhome();  // zero out in the corner
  step(1,20);  //step 40x in the X direction from home
  step2(0,50); //step 40x in the Y direction from home
      
  rack1();  //run rack position 1
  
  step2(0,118); //move in the X direction to the right - half way
  step(0,1100);  //move back to the front tubes - Y direction
  step2(0,118); //move in the X direction the rest of the way to the right


  rack1();  //run rack position 2
    digitalWrite(16, LOW); //open pinch valve to relieve pressure
  delay(5000);  //give it 5 seconds
  digitalWrite(16, HIGH); //close pinch valve
  //this program stops after the 2nd rack.

}

void masterRack3()  //this program executes from 'run3'. Home, then 3 racks.
{
  xyhome();  // zero out in the corner
  step(1,20);  //step 40x in the X direction from home
  step2(0,50); //step 40x in the Y direction from home
      
  rack1();  //run rack position 1
  
  step2(0,118); //move in the X direction to the right - half way
  step(0,1100);  //move back to the front tubes - Y direction
  step2(0,118); //move in the X direction the rest of the way to the right


  rack1();  //run rack position 2
  
  step2(0,115); //changed from 118 on 20180525
  step(0,1100);
  step2(0,115);


  rack1();  //run rack position 3
  digitalWrite(16, LOW); //open pinch valve to relieve pressure
  delay(5000);  //give it 5 seconds
  digitalWrite(16, HIGH); //close pinch valve
  //this program stops after the 3rd rack.

}
void masterRack1()  //this is the main program from pressing 'run1'. Home, then 4 racks.
{
  xyhome();  // zero out in the corner
  step(1,20);  //step 40x in the X direction from home
  step2(0,50); //step 40x in the Y direction from home
      
  rack1();  //run rack position 1
  digitalWrite(16, LOW); //open pinch valve to relieve pressure
  delay(5000);  //give it 5 seconds
  digitalWrite(16, HIGH); //close pinch valve 
  //this program stops after the 1st rack.

}
void rack5()
{
  xyhome3();
  step(0,10);  //step forward 10 steps
  step2(0,35); //step right 35 steps 

  digitalWrite(16, LOW); //open pinch valve
    digitalWrite(14, LOW); //turn on pump
    delay(Delay);  //for the first tube
  for (int index = 0; index < 7; index++) {  
    step2(0,162);  //step right 160 steps
    delay(Delay);  //for tubes 2-7 in the for loop
    }
  
  digitalWrite(16, HIGH); //close pinch valve
  digitalWrite(14, HIGH); //turn off pump
  step(0,90);
  step2(1,1110);
  step(0,92);
    digitalWrite(16, LOW); //open pinch valve
    digitalWrite(14, LOW); //turn on pump
  delay(Delay);  //need this delay for the first tube to fill, before entering the for loop
  
  for (int index = 0; index < 7; index++) {
    step2(0,162);
    delay(Delay);
    }
 
  digitalWrite(16, HIGH); //close pinch valve
  digitalWrite(14, HIGH); //turn off pump
  step(0,90);
  step2(1,1110);
  step(0,92);
    digitalWrite(16, LOW); //open pinch valve
    digitalWrite(14, LOW); //turn on pump
  delay(Delay);
    
  for (int index = 0; index < 7; index++) {
    step2(0,162);
    delay(Delay);
    }
  
  digitalWrite(14, HIGH); //turn off pump
  delay(5000);  //wait 5 seconds to relieve pressure in the column/lines
  digitalWrite(16, HIGH); //close pinch valve
  
}

void WholeEnchilada()
{
  masterRack4();
    step2(1,1800);
    step(1,400);
  rack5();  
  
}

