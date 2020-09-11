#include <Arduino.h>
#include <PID_v1.h>
#include <Encoder.h>
#include "Omni.h"



void setup()
{
	// put your setup code here, to run once:
	Serial.begin(9600); //Set the band rate to your Bluetooth module.
	pinMode(IN1A, OUTPUT);
	pinMode(IN2A, OUTPUT);
	pinMode(IN1B, OUTPUT);
	pinMode(IN2B, OUTPUT);
	pinMode(IN1C, OUTPUT);
	pinMode(IN2C, OUTPUT);
	pinMode(PWM1, OUTPUT);
	pinMode(PWM2, OUTPUT);
	pinMode(PWM3, OUTPUT);
	

	//initial check motor on start up
	w1(255, 1);
	w2(255, 1);
	w3(255, 1);
	delay(200);
	w1(0, 1);
	w2(0, 1);
	w3(0, 1);
	delay(1000);
	w1(255, -1);
	w2(255, -1);
	w3(255, -1);
	delay(200);
	w1(0, 1);
	w2(0, 1);
	w3(0, 1);
}

void loop()
{
	// put your main code here, to run repeatedly:
	control_PID(3,1);
}