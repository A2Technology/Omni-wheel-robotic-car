#include <Arduino.h>
#line 1 "d:\\Github\\Omni-wheel-robotic-car\\MainSketch.ino"
#include <stdio.h>
int led = 13;

// the setup routine runs once when you press reset:
#line 5 "d:\\Github\\Omni-wheel-robotic-car\\MainSketch.ino"
void setup();
#line 11 "d:\\Github\\Omni-wheel-robotic-car\\MainSketch.ino"
void loop();
#line 5 "d:\\Github\\Omni-wheel-robotic-car\\MainSketch.ino"
void setup() {
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);
}

// the loop routine runs over and over again forever:
void loop() {
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);               // wait for a second
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);               // wait for a second
}
