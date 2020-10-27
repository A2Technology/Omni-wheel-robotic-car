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
#define EN_1A 3 //change pin order to get positive value of encoder
#define EN_1B 2
#define EN_2A 19
#define EN_2B 18
#define EN_3A 21
#define EN_3B 20

void w1(int rotation, int direct);
void w2(int rotation, int direct);
void w3(int rotation, int direct);
void position(float x, float y, float w);
int sign_of(float x);
float read_speed(int select);
void control_ONOFF(float u, int select);
void control_PID(float u, int select);
long encoder_output(int select);
void PID_setup(void);
void bluetooth_controlPID(void);
void bluetooth_control(void);
//Return value of PID control input and output
float PID_output(int select);
float PID_input(int select);
struct Control
{
    //Output speed of 3 wheels
    float u1;
    float u2;
    float u3;
};
Control ControlMatrix(float x, float y, float w);

#endif
