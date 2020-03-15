// Copyright (C) 2017 EngineerDog.com LLC
//
// This work is licensed under the Creative Commons 
// Attribution-NonCommercial-ShareAlike 4.0 International License.
// To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-sa/4.0/.
// Attribution for derivations of this work should be made to: EngineerDog.com LLC
// 
// You may use this work for noncommercial purposes without cost as long as you give us
// credit for our work (attribution) and any improvements you make are licensed in a way
// no more restrictive than our license. See the license for more details.
//
// For example, you may build a SimpleSumo yourself and use this code for your own experiments,
// or you can build one and give the SimpleSumo running this code to a friend, as long as you
// don't charge for it. 
// If you have a question about whether a contemplated use is in compliance with the license,
// just ask us. We're friendly. Email us at MechEngineerMike.com
//
// For more information about SimpleSumo see https://engineerdog.com/simplesumo/,  
//
//***************************************************************************
//
// SimpleSumo Robotics "Sumo_Battle_Program"
// Original created September, 2017, by Michael Graham, www.EngineerDog.com
//
// This program operates the SimpleSumo robot in sumo wrestling mode.  
// The program is written to facilitate the customization of the SimpleSumo Robot modules & behaviour.
// Inputs are 2 buttons, 1 Ultrasonic Sensor, 3 edge sensors.
// Outputs are 2 continuous servo motors and a buzzer. Optional outputs include 2 180deg servos. 
//
// LEFT and RIGHT point of view defined by looking at top of robot when it is driving away from you. 
//
// The Circuit: 
//  A0 = 
//  A1 = 
//  A2 = 
//  A3 = Edge Sensor 'A0', Back
//  A4 = Edge Sensor 'A0', Right
//  A5 = Edge Sensor 'A0', Left
//  A6 = 
//  A7 = 
//  D0 = (reserved for Bluetooth connection later)
//  D1 = (reserved for Bluetooth connection later)
//  D2 = Buzzer long leg
//  D3~ = Continuous Servo, Right
//  D4 = 
//  D5~ = Continuous Servo, Left
//  D6~ = Ultrasonic 'Trig'
//  D7 = Ultrasonic 'Echo'
//  D8 = 
//  D9~ = Accessory Servo SG90, Right
//  D10~ = Button Input, Right
//  D11~ = Button Input, Left
//  D12 = Accessory Servo SG90, Left 
//  D13 = (reserved for the embedded LED on this pin)
//  Any 5V = Ultrasonic 'Vcc', 3x Edge Sensor 'Vcc', Servo Motor 'Vcc', 
//  Any GND = Ultrasonic 'GND', 3x  Edge Sensor 'GND', Servo Motor 'Gnd', 2x Button Output, Buzzer short leg

//***************************************************************************

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//____DECLARATIONS SECTION__________________________________________________________________________________________
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Servo.h>  //Arduino library that allows for simple control of servo motors. 

Servo LeftServo;   //Assigns a name to the left side continuous wheel servo
Servo RightServo;  //Assigns a name to the right side continuous wheel servo

const int RightServoPin = 3; // Right Continuous Servo, Digital Pin #
const int LeftServoPin = 5; // Left Continuous Servo, Digital Pin #

const int EdgeBackPin = 3;  //Edge Sensor Rear Center, Analog Pin #
const int EdgeRightPin = 4;  //Edge Sensor Front Left, Analog Pin #
const int EdgeLeftPin = 5;  //Edge Sensor Front Right, Analog Pin #

const int BuzzerPin = 2; //Buzzer sound output pin

const int EdgeSensitivity = 100;  //Between 0-1000, White surfaces read LOW, dark surfaces read High. This variable determines when line is crossed. Default = 500

int BackEdgeState = 0;    //Logic variable for recording if this edge was triggered or not
int RightEdgeState = 0;   //Logic variable for recording if this edge was triggered or not
int LeftEdgeState = 0;    //Logic variable for recording if this edge was triggered or not

bool EdgeSensed = false;

int StuckCounter = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//____SETUP SECTION_________________________________________________________________________________________________
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

 void setup()
{
 Serial.begin(9600);        // initialize serial communication at 9600 baud. The serial monitor is used for data collection.

 RightServo.attach(RightServoPin);  //Uses the servo library to set the right servo pin #
 RightServo.write(90);  // Continuous Servo: 90 sets servo speed to zero. (0 and 180 are the opposing extreme speeds)
 
 LeftServo.attach(LeftServoPin);  //Uses the servo library to set this pin # as the left servo
 LeftServo.write(90);  // Continuous Servo: 90 sets servo speed to zero. (0 and 180 are the opposing extreme speeds)
     
 pinMode(BuzzerPin, OUTPUT);
 
 Countdown();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//____ LOOP SECTION_________________________________________________________________________________________________________
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void loop(){
   // SitStill(); //This is a function written for troubleshooting purposes

  PrintSerial(); //Uncomment this only for troubleshooting. It will slow down your robots reaction time during a battle!
  CheckSensors(); //Review all input sensors for fresh data

  StayInRing(); // Move & reorient the robot away from ring edge

}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//____ FUNCTIONS SECTION___________________________________________________________________________________________________
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

 
void CheckSensors(){ //Review all input sensors for fresh data and set the robot state

  BackEdgeState = analogRead(EdgeBackPin);
  RightEdgeState = analogRead(EdgeRightPin);
  LeftEdgeState = analogRead(EdgeLeftPin);
 
  if (BackEdgeState < EdgeSensitivity || RightEdgeState < EdgeSensitivity || LeftEdgeState < EdgeSensitivity){
    EdgeSensed = true; //Edge positively sensed
    } else {
        EdgeSensed = false;
      }
}

void Buzz() { //Increasing the duration of the buzz changes the volume drastically. Set delay to 80+ms for a loud beep!   
  digitalWrite(BuzzerPin, HIGH);
  delay(5);
  digitalWrite(BuzzerPin, LOW);
}

void BuzzLong() {  //Increasing the duration of the buzz changes the volume drastically. Set delay to 80+ms for a loud beep!
  digitalWrite(BuzzerPin, HIGH);
  delay(50);
  digitalWrite(BuzzerPin, LOW);
}


void StayInRing(){   // Move & reorient the robot away from ring edge
  if (RightEdgeState < EdgeSensitivity && LeftEdgeState < EdgeSensitivity){
    MoveBackward();
      delay (500); //Duration of this motion in milliseconds
      RotateRight();
      delay (500); //Duration of this motion in milliseconds
    } else if (RightEdgeState < EdgeSensitivity){
        MoveBackwardRight();
        delay (500); //Duration of this motion in milliseconds
        RotateLeft(); 
        delay (500); //Duration of this motion in milliseconds
    } else if (LeftEdgeState < EdgeSensitivity){
        MoveBackwardLeft();
        delay (500); //Duration of this motion in milliseconds
        RotateRight(); 
        delay (500); //Duration of this motion in milliseconds
    } else {
      MoveForward();
      delay (125); //Duration of this motion in milliseconds
      }
}

void Countdown(){ //This pauses the robot for 5 seconds (5000 milliseconds) after it is turned on, per competition requirements. Then it beeps the 5 sec countdown. 
  Buzz();
  delay (995); 
  Buzz();
  delay (995); 
  Buzz();
  delay (995); 
  Buzz();
  delay (995); 
  BuzzLong();
  delay (950); 
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//____ MOTION COMMANDS (Just a sublevel of the functions)__________________________________________________________________
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void RotateRight(){
   LeftServo.write(90+30);  //(>90 is forward for left motor). 90 sets servo speed to zero. (0 and 180 are the opposing extreme speeds)
   RightServo.write(90+30);  //(<90 is forward for right motor). 90 sets servo speed to zero. (0 and 180 are the opposing extreme speeds)
} 

void RotateLeft(){
   LeftServo.write(90-30);  //(>90 is forward for left motor). 90 sets servo speed to zero. (0 and 180 are the opposing extreme speeds)
   RightServo.write(90-30);  //(<90 is forward for right motor). 90 sets servo speed to zero. (0 and 180 are the opposing extreme speeds)
} 

void SitStill(){
  LeftServo.write(90);  // (>90 is forward for left motor). 90 sets servo speed to zero. (0 and 180 are the opposing extreme speeds)
  RightServo.write(90);  // (<90 is forward for right motor). 90 sets servo speed to zero. (0 and 180 are the opposing extreme speeds)
}

void MoveBackward(){
  LeftServo.write(90-20);  // (>90 is forward for left motor). 90 sets servo speed to zero. (0 and 180 are the opposing extreme speeds)
  RightServo.write(90+20);  // (<90 is forward for right motor). 90 sets servo speed to zero. (0 and 180 are the opposing extreme speeds)
}

void MoveBackwardLeft(){
  LeftServo.write(90-20);  // (>90 is forward for left motor). 90 sets servo speed to zero. (0 and 180 are the opposing extreme speeds)
  RightServo.write(90+45);  // (<90 is forward for right motor). 90 sets servo speed to zero. (0 and 180 are the opposing extreme speeds)
}

void MoveBackwardRight(){
  LeftServo.write(90-45);  // (>90 is forward for left motor). 90 sets servo speed to zero. (0 and 180 are the opposing extreme speeds)
  RightServo.write(90+20);  // (<90 is forward for right motor). 90 sets servo speed to zero. (0 and 180 are the opposing extreme speeds)
}

void MoveForward(){
  LeftServo.write(90+15);  // (>90 is forward for left motor). 90 sets servo speed to zero. (0 and 180 are the opposing extreme speeds) 
  RightServo.write(90-15); // (<90 is forward for right motor). 90 sets servo speed to zero. (0 and 180 are the opposing extreme speeds) 
}

int PrintSerial()                        //This function prints all available data to the serial monitor
{
  Serial.print(millis()/1000);                // print the time in milliseconds since the program started
  Serial.print(',');                     // print a comma
  Serial.print(BackEdgeState);           // print the status of the back edge sensor
  Serial.print(',');                     // print a comma
  Serial.print(RightEdgeState);          // print the status of the right edge sensor
  Serial.print(',');                     // print a comma
  Serial.println(LeftEdgeState);         // print the status of the left edge sensor
}
