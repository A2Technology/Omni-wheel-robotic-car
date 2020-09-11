#include <Arduino.h>
#include <PID_v1.h>
#include <Encoder.h>
#include "Omni.h"

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

Encoder Encoder_1(EN_1A, EN_1B);
Encoder Encoder_2(EN_2A, EN_2B);
Encoder Encoder_3(EN_3A, EN_3B);

long interval = 1000; //choose interval is 1 second (1000 milliseconds)
long previousMillis = 0;
long currentMillis = 0;

long currentEncoder;
long previousEncoder;

float read_speed(int select)
{
    //read velocity of selected motor
    //return velocity in rad/s
    const int Encoder_1_round = 30000; //define number of pulses in one round of encoder
    switch (select)
    {
    case 1:
        currentEncoder = Encoder_1.read();
        break;
    case 2:
        currentEncoder = Encoder_2.read();
        break;
    case 3:
        currentEncoder = Encoder_3.read();
        break;
    }

    float rot_speed;
    currentMillis = millis();
    if (currentMillis - previousMillis > interval)
    {
        previousMillis = currentMillis;
        rot_speed = (float)abs(((currentEncoder - previousEncoder) * 2 * PI / Encoder_1_round));
        previousEncoder = currentEncoder;
        return rot_speed;
    }
}