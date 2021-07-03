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

// mouth moving code below. DO I NEED TO SWITCH 61 'IF' WITH 63 'IF'????

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

// head moving code below. 1/5/21 added timing logic to stop the head flapping due to LED pusles. Untested
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
