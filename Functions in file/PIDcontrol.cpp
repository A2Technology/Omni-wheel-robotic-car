#include <Encoder.h>
#include <PinChangeInt.h>
#include <PID_v1.h>
#define ENCODEROUTPUT 30000
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
void Plot(float x, float y, float w);
void w1(int rotation, int direct);
void w2(int rotation, int direct);
void w3(int rotation, int direct);
int sign_of(float x);
void control_PID(float u, int select);
float read_speed(int select);

double kp = 20, ki = 10, kd = 0.01, input_1 = 0, output_1 = 0, setpoint_1 = 0, input_2 = 0, output_2 = 0, setpoint_2 = 0, input_3 = 0, output_3 = 0, setpoint_3 = 0;
PID PID_1(&input_1, &output_1, &setpoint_1, kp, ki, kd, DIRECT);
PID PID_2(&input_2, &output_2, &setpoint_2, kp, ki, kd, DIRECT);
PID PID_3(&input_3, &output_3, &setpoint_3, kp, ki, kd, DIRECT);

void setup()
{
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

    PID_1.SetMode(AUTOMATIC);
    PID_1.SetSampleTime(1);
    PID_1.SetOutputLimits(0, 255);

    PID_2.SetMode(AUTOMATIC);
    PID_2.SetSampleTime(1);
    PID_2.SetOutputLimits(0, 255);

    PID_3.SetMode(AUTOMATIC);
    PID_3.SetSampleTime(1);
    PID_3.SetOutputLimits(0, 255);
}




void control_PID(float u, int select)
{
    //Get sign of rotating velocity of wheels
    int u_sign = sign_of(u);

    switch (select)
    {
    case 1:
        setpoint_1 = u;
        input_1 = read_speed(1);
        PID_1.Compute();
        w1(output_1, u_sign);
        break;

    case 2:
        setpoint_2 = u;
        input_2 = read_speed(2);
        PID_2.Compute();
        w2(output_2, u_sign);
        break;

    case 3:
        setpoint_3 = u;
        input_3 = read_speed(3);
        PID_3.Compute();
        w3(output_3, u_sign);
        break;
    }
}





