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
	w1(255, 1);
	w2(255, 1);
	w3(255, 1);
	delay(200);
	w1(0, 1);
	w2(0, 1);
	w3(0, 1);
	delay(2000);
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
	control_PID(3, 1);
	control_PID(3, 2);
	control_PID(3, 3);
	if (read_speed(1)!=0)
	{
	Serial.print("1  ");
	Serial.println(read_speed(1));
	}
	if (read_speed(2)!=0)
	{
	Serial.print("2  ");
	Serial.println(read_speed(2));
	}
	if (read_speed(3)!=0)
	{
	Serial.print("3  ");
	Serial.println(read_speed(3));
	}

	

	// // put your main code here, to run repeatedly:
	// command = Serial.read();
	// Serial.println(command);

	// // Serial.println(command);
	// switch (command)
	// {
	// //Adjust speed
	// case '1':
	// 	speedcar = 0.3;
	// 	break;
	// case '2':
	// 	speedcar = 0.35;
	// 	break;
	// case '3':
	// 	speedcar = 0.4;
	// 	break;
	// case '4':
	// 	speedcar = 0.45;
	// 	break;
	// case '5':
	// 	speedcar = 0.5;
	// 	break;
	// case '6':
	// 	speedcar = 0.6;
	// 	break;
	// case '7':
	// 	speedcar = 0.7;
	// 	break;
	// case '8':
	// 	speedcar = 0.8;
	// 	break;
	// case '9':
	// 	speedcar = 0.9;
	// 	break;
	// case '0':
	// 	speedcar = 0;
	// 	break;
	// case 'q':
	// 	speedcar = 1;
	// 	break;
	//Control motion
	// case 'F': //Forward
	// 	control_PID(0, 1);
	// 	control_PID(-255 * speedcar, 2);
	// 	control_PID(255 * speedcar, 3);
	// 	break;

	// case 'B': //Backward
	// 	control_PID(0, 1);
	// 	control_PID(255 * speedcar, 2);
	// 	control_PID(-255 * speedcar, 3);
	// 	break;

	// case 'R': //Right
	// 	control_PID(-255 * speedcar, 1);
	// 	control_PID(180 * speedcar, 2);
	// 	control_PID(180 * speedcar, 3);
	// 	break;

	// case 'L': //Left
	// 	control_PID(255 * speedcar, +1);
	// 	control_PID(-180 * speedcar, 2);
	// 	control_PID(-180 * speedcar, 3);
	// 	break;

	// case 'G': //Forward left
	// 	control_PID(186 * speedcar, 1);
	// 	control_PID(-255 * speedcar, 2);
	// 	control_PID(68 * speedcar, 3);
	// 	break;

	// case 'I': //Forward right
	// 	control_PID(-186 * speedcar, 1);
	// 	control_PID(-255 * speedcar, 2);
	// 	control_PID(68 * speedcar, 3);
	// 	break;

	// case 'H': //Backward left
	// 	control_PID(186 * speedcar, 1);
	// 	control_PID(68 * speedcar, 2);
	// 	control_PID(-255 * speedcar, 3);
	// 	break;

	// case 'J': //Backward right
	// 	control_PID(-186 * speedcar, 1);
	// 	control_PID(255 * speedcar, 2);
	// 	control_PID(-68 * speedcar, 3);
	// 	break;

	// case 'S': //No motor input
	// 	control_PID(0, 1);
	// 	control_PID(0, 2);
	// 	control_PID(0, 3);
	// 	break;

	// case 'W': //Rotate left
	// 	control_PID(255 * speedcar, 1);
	// 	control_PID(255 * speedcar, 2);
	// 	control_PID(255 * speedcar, 3);
	// 	delay(200);
	// 	break;

	// case 'w': //Rotate left
	// 	control_PID(255 * speedcar, 1);
	// 	control_PID(255 * speedcar, 2);
	// 	control_PID(255 * speedcar, 3);
	// 	delay(200);
	// 	break;

	// case 'U': //Rotate right
	// 	control_PID(-255 * speedcar, 1);
	// 	control_PID(-255 * speedcar, 2);
	// 	control_PID(-255 * speedcar, 3);
	// 	delay(200);
	// 	break;

	// case 'u': //Rotate right
	// 	control_PID(-255 * speedcar, 1);
	// 	control_PID(-255 * speedcar, 2);
	// 	control_PID(-255 * speedcar, 3);
	// 	delay(200);
	// 	break;

	// case 'F': //Forward
	// 	w1(0, 1);
	// 	w2(255 * speedcar, -1);
	// 	w3(255 * speedcar, 1);
	// 	break;

	// case 'B': //Backward
	// 	w1(0, +1);
	// 	w2(255 * speedcar, 1);
	// 	w3(255 * speedcar, -1);
	// 	break;

	// case 'R': //Right
	// 	w1(255 * speedcar, -1);
	// 	w2(180 * speedcar, +1);
	// 	w3(180 * speedcar, +1);
	// 	break;

	// case 'L': //Left
	// 	w1(255 * speedcar, +1);
	// 	w2(180 * speedcar, -1);
	// 	w3(180 * speedcar, -1);
	// 	break;

	// case 'G': //Forward left
	// 	w1(186 * speedcar, +1);
	// 	w2(255 * speedcar, -1);
	// 	w3(68 * speedcar, +1);
	// 	break;

	// case 'I': //Forward right
	// 	w1(186 * speedcar, -1);
	// 	w2(255 * speedcar, -1);
	// 	w3(68 * speedcar, +1);
	// 	break;

	// case 'H': //Backward left
	// 	w1(186 * speedcar, +1);
	// 	w2(68 * speedcar, +1);
	// 	w3(255 * speedcar, -1);
	// 	break;

	// case 'J': //Backward right
	// 	w1(186 * speedcar, -1);
	// 	w2(255 * speedcar, +1);
	// 	w3(68 * speedcar, -1);
	// 	break;

	// case 'S': //No motor input
	// 	w1(0, +1);
	// 	w2(0, +1);
	// 	w3(0, -1);
	// 	break;

	// case 'W': //Rotate left
	// 	w1(255 * speedcar, +1);
	// 	w2(255 * speedcar, +1);
	// 	w3(255 * speedcar, +1);
	// 	delay(200);
	// 	break;

	// case 'w': //Rotate left
	// 	w1(255 * speedcar, +1);
	// 	w2(255 * speedcar, +1);
	// 	w3(255 * speedcar, +1);
	// 	delay(200);
	// 	break;

	// case 'U': //Rotate right
	// 	w1(255 * speedcar, -1);
	// 	w2(255 * speedcar, -1);
	// 	w3(255 * speedcar, -1);
	// 	delay(200);
	// 	break;

	// case 'u': //Rotate right
	// 	w1(255 * speedcar, -1);
	// 	w2(255 * speedcar, -1);
	// 	w3(255 * speedcar, -1);
	// 	delay(200);
	// 	break;

	// case 'X':
	// 	//Go square
	// 	Plot(1, 0, 0);
	// 	delay(100);
	// 	Plot(0, 1, 0);
	// 	delay(100);
	// 	Plot(-1, 0, 0);
	// 	delay(100);
	// 	Plot(0, -1, 0);
	// 	delay(100);
	// 	break;

	// case 'D':
	// 	w1(0, 1);
	// 	w2(0, 1);
	// 	w3(0, 1);
	// 	break;

	// default:
	// 	break;
	// }
}