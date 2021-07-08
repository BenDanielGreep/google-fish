## Table of contents
* [General info](#general-info)
* [Technologies](#technologies)
* [Setup](#setup)
* [Guide](#guide)

## General info
This project was my first foray into coding! I'm really excited to share this code. There are one or two guides out there, but they had a fatal flaw; the audio signal was beign fed into the Arduino which is VERY dangerous. It contains negative voltage which will damage and destroy your Arduino. The way around this was to install a sound detection module to the Arduino and place it in proximity to the Google Next unit inside the fish. It means that technically the system is open to influence from outside noises, but even the loudest sounds don't seem to upset the fish once the levels are properly set in testing. 
	
## Technologies
Project is created with:
* C++
* Arduino Uno, with motor shield, sound detection module, light detection module
* Google Nest Mini
* Big Mouth Billy Bass
	
## Setup
Before uploading the code, be sure to have the correct wiring for your Arduino (full instructable guide coming soon).
Then simply plug your Arduino Uno into your computer, boot up the Arduino program and upload the code. 
I'll explain what it does below.

## Guide

The first step is declaring your inputs, outputs and counters. Things that are going to be referenced while the code runs.
* AFMotor is a library that allows control of the motors, which you assign to the corresponding pins on the Arduino.
* You can see that the interval timers can (and probably will)  need adjusting as you test your setup, due to the narrow threshold the audio and light sensors operate within. 

```
#include <AFMotor.h>

AF_DCMotor mouth(1);
AF_DCMotor head(2); 

int audioPin = A5;    // select the input pin for the audio signal input
int lightPin = A0; // input pin for light sensor
int ledPin = 13; // used for testing

int lightThreshold = 0;
int audioThreshold = 0;

// these two numbers define how the mouth moves, and require tinkering to get to work properly
int audioInterval = 50; // time that mouth motor runs to open sufficiently
int mouthInterval = 51; //time before the mouth can open again
int headInterval = 2000; // time that the head will stay open. Created as the sensor is oversensitive to the pulse of the LEDs

unsigned long audioMillis = 0;
unsigned long headMillis = 0;
unsigned long previousMillis = 0;
unsigned long previousHeadMillis = 0;

unsigned long currentMouth_Millis = 0; // stores the value of millis() in each iteration of loop()
unsigned long previousMouth_Millis = 0; // will store the last time the mouth moved
unsigned long currentCloseMouth_Millis = 0;
unsigned long previousCloseMouth_Millis = 0;

```

When you first turn on the Arduino, it runs the setup code _once_
* setting inputs and outputs for the LEDs is self-explanatory. 
* The mouth speed (255) is the max speed for the motors, and I run them so that you can see they work and to make sure they're in the correct position before running fully.

```

void setup() {
  // put your setup code here, to run once:
  pinMode(ledPin, OUTPUT);
  pinMode(audioPin, INPUT);
  pinMode(lightPin, INPUT);
  Serial.begin(9600);  // open the serial port at 9600 bps: 
  mouth.setSpeed(255);
  mouth.run(RELEASE);
  mouth.run(FORWARD);
  delay(500);
  mouth.run(RELEASE); 
  delay(500);
  head.setSpeed(255);
  head.run(RELEASE);
  head.run(BACKWARD);
  delay(500);
  head.run(RELEASE);
}

```

Now for the loop- this is the main part of the code that actually does things.
* We start a counter of milliseconds. This allows the Arduino to do many things at once. I learned the concept from the Arudino forum: https://forum.arduino.cc/t/demonstration-code-for-several-things-at-the-same-time/217158/2. It can look a little complex, but it's Arduino's way of keeping track of time and essentially providing multitasking. 
* Essentially, all that we're doing is saying 'If google is talking, move the mouth. If the light is on, move the head'
```

void loop() {
  //check for theshold, then run motor until interval has passed then release and check again
  int lightValue = digitalRead(lightPin);
  int audioValue = digitalRead(audioPin);
  unsigned long currentMillis = millis();

  
  Serial.print("audioValue: ");  // print the audio value, can remove
  Serial.println(audioValue); // 
  delay(50);

//  Serial.print("lightValue: ");  // print the light value, can remove
//  Serial.println(lightValue); // 0 is 'light detected', 1 is 'no light detected'


 if (currentMillis - audioMillis >= audioInterval) {        // if more than 50 ms has passed
    audioMillis = currentMillis;
    if (audioValue == audioThreshold) {                     // if Google is talking

      if (currentMillis - previousMillis >= mouthInterval) {  // if more than 51ms has passed
        previousMillis = currentMillis;
        mouth.run(FORWARD);
      }
      else {
        mouth.run(RELEASE);
      }
    }

    else {
      mouth.run(RELEASE);
      }
    }

 if  (lightValue == lightThreshold) {
    headMillis = currentMillis; //for timing
    head.run(BACKWARD);
 
   if (currentMillis - headMillis >= headInterval) { // if more than 2000ms has passed
    previousHeadMillis = currentMillis;
      head.run(RELEASE);
   }
   else {
    head.run(BACKWARD);
   }
 }
  else {
    head.run(RELEASE);
  }
 }
}
```
Enjoy this project!
