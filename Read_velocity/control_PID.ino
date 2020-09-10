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

double x, y, w;
int u1_sign, u2_sign, u3_sign;
const byte speedcar = 1;
char command;
int scale = 1;
int StError = 0;

long oldPosition_1 = 0;
long newPosition_1;
long oldPosition_2 = 0;
long newPosition_2;
long oldPosition_3 = 0;
long newPosition_3;

long interval = 1000;
long previousMillis = 0;
long currentMillis = 0;


long CurrentEncoder;
long previousEncoder;

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
void loop()
{
    control_PID(5, 1);
}

void Plot(float x, float y, float w)
{
    ////Go with direction in Oxy frame.
    //x is relative X coordinate
    //y is relative Y coordinate
    //w is relative angle of rotation

    //motor value
    float u1 = (175 * w - 1000 * x) / 53;
    float u2 = (175 * w + 500 * x - 500 * sqrt(3) * y) / 53;
    float u3 = (175 * w + 500 * x + 500 * sqrt(3) * y) / 53;

    //Get sign of rotating velocity of wheels
    u1_sign = sign_of(u1);
    u2_sign = sign_of(u2);
    u3_sign = sign_of(u3);

    //Because control signal is positive number, we take abs of u1 u2 u3
    u1 = abs(u1);
    u2 = abs(u2);
    u3 = abs(u3);

    //Mapping value
    float M_in_min = 0;
    float M_in_max = max(max(u1, u2), u3);
    float M_out_min = 0;
    float M_out_max = 255;

    //Remapping motors' value
    u1 = (u1 - M_in_min) * (M_out_max - M_out_min) / (M_in_max - M_in_min) + M_out_min;
    u2 = (u2 - M_in_min) * (M_out_max - M_out_min) / (M_in_max - M_in_min) + M_out_min;
    u3 = (u3 - M_in_min) * (M_out_max - M_out_min) / (M_in_max - M_in_min) + M_out_min;

    float p_u1 = (175 * w - 1000 * x) / 53;
    float p_u2 = (175 * w + 500 * x - 500 * sqrt(3) * y) / 53;
    float p_u3 = (175 * w + 500 * x + 500 * sqrt(3) * y) / 53;

    do
    {
        long newPosition_1 = abs(Encoder_1.read());
        long newPosition_2 = abs(Encoder_2.read());
        long newPosition_3 = abs(Encoder_3.read());

        if (newPosition_1 < abs(scale * p_u1 * 30000 / (2 * PI)) + StError)
            w1(u1, u1_sign);
        else
            w1(0, 0);

        if (newPosition_2 < abs(scale * p_u2 * 30000 / (2 * PI)) + StError)
            w2(u2, u2_sign);
        else
            w2(0, 0);

        if (newPosition_3 < abs(scale * p_u3 * 30000 / (2 * PI)) + StError)
            w3(u3, u3_sign);
        else
            w3(0, 0);

        if (newPosition_1 != oldPosition_1)
        {
            oldPosition_1 = newPosition_1;
        }
        if (newPosition_2 != oldPosition_2)
        {
            oldPosition_2 = newPosition_2;
        }
        if (newPosition_3 != oldPosition_3)
        {
            oldPosition_3 = newPosition_3;
        }
        if (!(newPosition_1 < abs(scale * p_u1 * 30000 / (2 * PI)) + StError || newPosition_2 < abs(scale * p_u2 * 30000 / (2 * PI)) + StError || newPosition_3 < abs(scale * p_u3 * 30000 / (2 * PI)) + StError))
            break;
    } while (newPosition_1 < abs(scale * p_u1 * 30000 / (2 * PI)) + StError || newPosition_2 < abs(scale * p_u2 * 30000 / (2 * PI)) + StError || newPosition_3 < abs(scale * p_u3 * 30000 / (2 * PI)) + StError);
}

int sign_of(float x)
{
    if (x >= 0)
        return 1;
    else
        return -1;
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
void w1(int rotation, int direct)
{
    //Motor 1 control
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
    //Motor 2 control
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
    //Motor 3 control
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
float read_speed(int select)
{
    //read velocity of selected motor
    //return velocity in rad/s
    switch (select)
    {
    case 1:
        CurrentEncoder = Encoder_1.read();
        break;
    case 2:
        CurrentEncoder = Encoder_2.read();
        break;
    case 3:
        CurrentEncoder = Encoder_3.read();
        break;
    }

    float rot_speed;
    currentMillis = millis();
    if (currentMillis - previousMillis > interval)
    {
        previousMillis = currentMillis;

        rot_speed = (float)abs(((CurrentEncoder - previousEncoder) * 2 * PI / ENCODEROUTPUT));
        
        previousEncoder = CurrentEncoder;

        return rot_speed;
    }
}
