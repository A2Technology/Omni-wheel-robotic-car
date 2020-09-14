#include <Arduino.h>
#include <PID_v1.h>
#include <Encoder.h>
#include "Omni.h"

char command;
float speedcar = 0.5;
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
	w1(255*speedcar, 1);
	w2(255*speedcar, 1);
	w3(255*speedcar, 1);
	delay(200);
	w1(0, 1);
	w2(0, 1);
	w3(0, 1);
	delay(2000);
	w1(255*speedcar, -1);
	w2(255*speedcar, -1);
	w3(255*speedcar, -1);
	delay(200);
	w1(0, 1);
	w2(0, 1);
	w3(0, 1);
}

void loop()
{
	// put your main code here, to run repeatedly:
	command = Serial.read();
	Serial.println(command);

	// Serial.println(command);
	switch (command)
	{
	case 'F': //Forward
		w1(0, 1);
		w2(255 * speedcar, -1);
		w3(255 * speedcar, 1);
		break;

	case 'B': //Backward
		w1(0, +1);
		w2(255 * speedcar, 1);
		w3(255 * speedcar, -1);
		break;

	case 'R': //Right
		w1(255 * speedcar, -1);
		w2(180 * speedcar, +1);
		w3(180 * speedcar, +1);
		break;

	case 'L': //Left
		w1(255 * speedcar, +1);
		w2(180 * speedcar, -1);
		w3(180 * speedcar, -1);
		break;

	case 'G': //Forward left
		w1(186 * speedcar, +1);
		w2(255 * speedcar, -1);
		w3(68 * speedcar, +1);
		break;

	case 'I': //Forward right
		w1(186 * speedcar, -1);
		w2(255 * speedcar, -1);
		w3(68 * speedcar, +1);
		break;

	case 'H': //Backward left
		w1(186 * speedcar, +1);
		w2(68 * speedcar, +1);
		w3(255 * speedcar, -1);
		break;

	case 'J': //Backward right
		w1(186 * speedcar, -1);
		w2(255 * speedcar, +1);
		w3(68 * speedcar, -1);
		break;

	case 'S': //No motor input
		w1(0, +1);
		w2(0, +1);
		w3(0, -1);
		break;

	case 'W': //Rotate left
		w1(255 * speedcar, +1);
		w2(255 * speedcar, +1);
		w3(255 * speedcar, +1);
		delay(200);
		break;

	case 'w': //Rotate left
		w1(255 * speedcar, +1);
		w2(255 * speedcar, +1);
		w3(255 * speedcar, +1);
		delay(200);
		break;

	case 'U': //Rotate right
		w1(255 * speedcar, -1);
		w2(255 * speedcar, -1);
		w3(255 * speedcar, -1);
		delay(200);
		break;

	case 'u': //Rotate right
		w1(255 * speedcar, -1);
		w2(255 * speedcar, -1);
		w3(255 * speedcar, -1);
		delay(200);
		break;

	case 'X':
		//Go square
		Plot(1, 0, 0);
		delay(100);
		Plot(0, 1, 0);
		delay(100);
		Plot(-1, 0, 0);
		delay(100);
		Plot(0, -1, 0);
		delay(100);
		break;

	case 'D':
		w1(0, 1);
		w2(0, 1);
		w3(0, 1);
		break;

	default:
		break;
	}
}