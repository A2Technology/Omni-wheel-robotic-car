#include <Arduino.h>
#include <PID_v1.h>
#include <Encoder.h>
#include "Omni.h"
double kp = 20, ki = 10, kd = 0.01, input_1 = 0, output_1 = 0, setpoint_1 = 0, input_2 = 0, output_2 = 0, setpoint_2 = 0, input_3 = 0, output_3 = 0, setpoint_3 = 0;
PID PID_1(&input_1, &output_1, &setpoint_1, kp, ki, kd, DIRECT);
PID PID_2(&input_2, &output_2, &setpoint_2, kp, ki, kd, DIRECT);
PID PID_3(&input_3, &output_3, &setpoint_3, kp, ki, kd, DIRECT);
Encoder Encoder_1(EN_1A, EN_1B);
Encoder Encoder_2(EN_2A, EN_2B);
Encoder Encoder_3(EN_3A, EN_3B);
long previousMillis = 0;
long currentMillis = 0;

volatile long currentEncoder;
volatile long previousEncoder;
volatile long oldPosition_1 = 0;
volatile long oldPosition_2 = 0;
volatile long oldPosition_3 = 0;
volatile long newPosition_1;
volatile long newPosition_2;
volatile long newPosition_3;

void control_PID(float u, int select)
{
    PID_1.SetMode(AUTOMATIC);
    PID_1.SetSampleTime(1);
    PID_1.SetOutputLimits(0, 255);

    PID_2.SetMode(AUTOMATIC);
    PID_2.SetSampleTime(1);
    PID_2.SetOutputLimits(0, 255);

    PID_3.SetMode(AUTOMATIC);
    PID_3.SetSampleTime(1);
    PID_3.SetOutputLimits(0, 255);
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

void control_ONOFF(float u, int select)
{
    //control motor speed using ON-OFF control
    float k = 0.1; //Range of accepted value [u(1-k),u(1+k)]
    float v = 0;   //velocity read

    switch (select) //Select motor
    {
    case 1:
        v = read_speed(1);
        break;
    case 2:
        v = read_speed(2);
        break;
    case 3:
        v = read_speed(3);
        break;
    }

    if (v > u + u * k) //upper threshold
        switch (select)
        {
        case 1:
            w1(128, sign_of(u));
            break;
        case 2:
            w2(128, sign_of(u));
            break;
        case 3:
            w3(128, sign_of(u));
            break;
        }

    else if (v < u - u * k) //lower threshold
        switch (select)
        {
        case 1:
            w1(255, sign_of(u));
            break;
        case 2:
            w2(255, sign_of(u));
            break;
        case 3:
            w3(255, sign_of(u));
            break;
        }
}

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
    const int interval = 1000; //choose interval is 1 second (1000 milliseconds)
    currentMillis = millis();
    if (currentMillis - previousMillis > interval)
    {
        previousMillis = currentMillis;
        rot_speed = (float)abs(((currentEncoder - previousEncoder) * 2 * PI / Encoder_1_round));
        previousEncoder = currentEncoder;
        return rot_speed;
    }
    else
        return 0;
}

void Plot(float x, float y, float w)
{
    const float r = 0.175;
    const float l = 0.053;
    const int StError = 0;
    const int scale = 1;
    //motor value
    float u1 = (l * w - x) / r;
    float u2 = (2 * l * w + x - sqrt(3) * y) / 2 * r;
    float u3 = (2 * l * w + x + sqrt(3) * y) / 2 * r;

    //Get sign of rotating velocity of wheels
    int u1_sign, u2_sign, u3_sign;
    u1_sign = sign_of(u1);
    u2_sign = sign_of(u2);
    u3_sign = sign_of(u3);

    //Because control signal is positive number, we take abs of u1 u2 u3
    u1 = abs(u1);
    u2 = abs(u2);
    u3 = abs(u3);

    //Mapping value
    float M_in_min = 0;
    //M_in_max = 19;
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

        newPosition_1 = abs(Encoder_1.read());
        newPosition_2 = abs(Encoder_2.read());
        newPosition_3 = abs(Encoder_3.read());

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
            // Serial.print("Encoder 1: ");
            // Serial.println(newPosition_1);
        }
        //
        //
        if (newPosition_2 != oldPosition_2)
        {
            oldPosition_2 = newPosition_2;
            // Serial.print("Encoder 2: ");
            // Serial.println(newPosition_2);
        }
        //
        //
        if (newPosition_3 != oldPosition_3)
        {
            oldPosition_3 = newPosition_3;
            // Serial.print("Encoder 3: ");
            // Serial.println(newPosition_3);
        }

        if (!(newPosition_1 < abs(scale * p_u1 * 30000 / (2 * PI)) + StError || newPosition_2 < abs(scale * p_u2 * 30000 / (2 * PI)) + StError || newPosition_3 < abs(scale * p_u3 * 30000 / (2 * PI)) + StError))
            break;

    } while (newPosition_1 < abs(scale * p_u1 * 30000 / (2 * PI)) + StError || newPosition_2 < abs(scale * p_u2 * 30000 / (2 * PI)) + StError || newPosition_3 < abs(scale * p_u3 * 30000 / (2 * PI)) + StError);

    newPosition_1 = newPosition_2 = newPosition_3 = 0;
    Encoder_1.write(0);
    Encoder_2.write(0);
    Encoder_3.write(0);

    // Serial.print("Encoder 1: ");
    // Serial.println(newPosition_1);
    // Serial.print("Encoder 2: ");
    // Serial.println(newPosition_2);
    // Serial.print("Encoder 3: ");
    // Serial.println(newPosition_3);
}
