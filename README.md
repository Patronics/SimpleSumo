# SimpleSumo
Arduino code for the SimpleSumo robot lineup.

The SimpleSumo is a design from Engineer Mike "MechEngineerMike" at https://engineerdog.com/

The instructable that he created that we're using is at: https://www.instructables.com/id/SimpleSumo-Educational-Fighting-Robots/

This robot can be built for approximately $30 for individual parts. Many of the parts though are only available in multi-packs which will present an increased cost burden for builders.

Here is the Bill of Materials that we used for this project: https://docs.google.com/spreadsheets/d/1fTZmPjsJM4xbr6QlCq4lyasEEuuKHJx6P3JwXfj3H0w

Summary of programs:

**Sumo_Battle_Program**
//The standard program used for display sumo battles. This program uses algotrythms designed to make a battle at a party/event more entertaining, even at the cost of battle performace. As such there are some simple things you can do to further optimize the robot. (These are good cantidats to have students pick out. For example the 'stuck counter' and the excessive number of random movements can be improved upon.)


**Battle_Program_Less_Wild**
//The standard battle program is good for display at events, but it is suboptimal for making youtube videos or displaying a science theory to a classroom. 
Features Removed:
 -The random movements from the search algorythm. Now sumo only goes straight unless it hits an edge.
 -The need to press the blade button to get started. Now the countdown begins immediately.
 -The stuck counter.   Removing this allows me to let the sumo charge forward indefinitely.


**Flee_Program**
 // This program operates the SimpleSumo robot in 'Flee Mode' so that it will run from walls as sensed with the ultrasonic & button sensors. If captured (as sensed by the IR signal from being lifted off the ground) the robot will stop rotating its wheels and beep.	
   
   This is a program segment intended to be used in conjunction with the "Sumo_Battle_Program". To make this work copy all of the code replace the entire 'SETUP SECTION' and 'LOOP SECTION' in the "Sumo_Battle_Program".

**ForwardLineDetect**
// This addition is simply to drive forward, detect the line, backup and turn around and drive forward again.  In the folder is also a state table in powerpoint. An image is [here](./ForwardLineDetect/ForwardLineDetectStateFlowChart.png).  This helps a new user to get started with basic use of the servos and sensors.