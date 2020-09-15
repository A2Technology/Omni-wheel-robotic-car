#ifndef Omni_h
#define Omni_h

#include <Arduino.h>
#include <PID_v1.h>
#include <Encoder.h>

#define IN1A 22
#define IN2A 23
#define IN1B 24
#define IN2B 25
#define IN1C 26
#define IN2C 27
#define PWM1 11
#define PWM2 12
#define PWM3 13
#define EN_1A 2
#define EN_1B 3
#define EN_2A 18
#define EN_2B 19
#define EN_3A 20
#define EN_3B 21
 


void w1(int rotation, int direct);
void w2(int rotation, int direct);
void w3(int rotation, int direct);
void position(float x, float y, float w);
int sign_of(float x);
float read_speed(int select);
void control_ONOFF(float u, int select);
void control_PID(float u, int select);
long encoder_output(int select);

#endif
