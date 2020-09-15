#include <Arduino.h>
#include <PID_v1.h>
#include <Encoder.h>
#include "Omni.h"

void w1(int rotation, int direct)
{
    //Control rotation of wheel 1
    //variable "rotation" gets value from 0 (0% power) to 255 (100% power)
    analogWrite(PWM1, rotation);
    if (direct == 1)
    {
        digitalWrite(IN1A, HIGH);
        digitalWrite(IN2A, LOW);
    }
    else if (direct == -1)
    {
        digitalWrite(IN1A, LOW);
        digitalWrite(IN2A, HIGH);
    }
}

void w2(int rotation, int direct)
{
    //Control rotation of wheel 2
    //variable "rotation" gets value from 0 (0% power) to 255 (100% power)
    analogWrite(PWM2, rotation);
    if (direct == 1)
    {
        digitalWrite(IN1B, HIGH);
        digitalWrite(IN2B, LOW);
    }
    else if (direct == -1)
    {
        digitalWrite(IN1B, LOW);
        digitalWrite(IN2B, HIGH);
    }
}

void w3(int rotation, int direct)
{
    //Control rotation of wheel 3
    //variable "rotation" gets value from 0 (0% power) to 255 (100% power)
    analogWrite(PWM3, rotation);
    if (direct == 1)
    {
        digitalWrite(IN1C, HIGH);
        digitalWrite(IN2C, LOW);
    }
    else if (direct == -1)
    {
        digitalWrite(IN1C, LOW);
        digitalWrite(IN2C, HIGH);
    }
}
