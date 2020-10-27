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
	pinMode(EN_1A, INPUT_PULLUP);
	pinMode(EN_1B, INPUT_PULLUP);
	pinMode(EN_2A, INPUT_PULLUP);
	pinMode(EN_2B, INPUT_PULLUP);
	pinMode(EN_3A, INPUT_PULLUP);
	pinMode(EN_3B, INPUT_PULLUP);
	PID_setup();

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
	// float speedd = 0;
	// w1(255, +1);
	// w2(255, -1);
	// w3(255, -1);

	// float v1 = read_speed(1);
	// float v2 = read_speed(2);
	// float v3 = read_speed(3);
	// long p1 = encoder_output(1);
	// long p2 = encoder_output(2);
	// long p3 = encoder_output(3);

	// //float input = encoder_output(3); //PID_input(2);

	// Serial.print("Pos ");
	// Serial.print(p1);
	// Serial.print("  ");
	// Serial.print(p2);
	// Serial.print("  ");
	// Serial.println(p3);

	// Serial.print("Vel ");
	// Serial.print(v1);
	// Serial.print("  ");
	// Serial.print(v2);
	// Serial.print("  ");
	// Serial.println(v3);
	bluetooth_control();
}


