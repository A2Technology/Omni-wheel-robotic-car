#include <Arduino.h>
#include <PID_v1.h>
#include <Encoder.h>
#include "Omni.h"
//Set PID constants
double kp = 50, ki = 20, kd = 5;
double input_1 = 0, output_1 = 0, setpoint_1 = 0;
double input_2 = 0, output_2 = 0, setpoint_2 = 0;
double input_3 = 0, output_3 = 0, setpoint_3 = 0;

//Declare PID functions
PID PID_1(&input_1, &output_1, &setpoint_1, kp, ki, kd, DIRECT);
PID PID_2(&input_2, &output_2, &setpoint_2, kp, ki, kd, DIRECT);
PID PID_3(&input_3, &output_3, &setpoint_3, kp, ki, kd, DIRECT);

//Declare encoder functions
Encoder Encoder_1(EN_1A, EN_1B);
Encoder Encoder_2(EN_2A, EN_2B);
Encoder Encoder_3(EN_3A, EN_3B);

//Time variables
long previousMillis = 0;
long currentMillis = 0;

//Encoder variables
volatile long currentEncoder;
volatile long previousEncoder = 0;
volatile long oldPosition_1 = 0;
volatile long oldPosition_2 = 0;
volatile long oldPosition_3 = 0;
volatile long newPosition_1;
volatile long newPosition_2;
volatile long newPosition_3;

//Bluetooth constant
char command;
float speedcar = 0.5;

float v = 0;

Control ControlMatrix(float x, float y, float w)
{
    //x relative coordinate
    //y relative coordinate
    //w angle of rotation
    //u1 u2 u3 are speed of 3 wheels 1 2 3
    const float r = 0.053; //radius of wheels in meters
    const float L = 0.175; //distance from center of the car to wheels in meters
    float u1, u2, u3;
    u1 = (L * w - x) / r;
    u2 = (2 * L * w + x - sqrt(3) * y) / (2 * r);
    u3 = (2 * L * w + x + sqrt(3) * y) / (2 * r);

    //Return 3 values
    Control wheel_speed = {u1, u2, u3};
    return wheel_speed;
}

void PID_setup(void)
{
    //PID initial settings
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

float PID_output(int select)
{
    switch (select)
    {
    case 1:
        return output_1;
        break;

    case 2:
        return output_2;
        break;

    case 3:
        return output_3;
        break;

    default:
        return 0;
        break;
    }
}
float PID_input(int select)
{
    switch (select)
    {
    case 1:
        return input_1;
        break;

    case 2:
        return input_2;
        break;

    case 3:
        return input_3;
        break;

    default:
        return 0;
        break;
    }
}

void control_PID(float u, int select)
{
    //control the wheel using PID
    int u_sign = sign_of(u); //Get sign of rotating velocity of wheels
    u = abs(u);              //get the absolute value of input speed (because we have the variable u_sign)
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
    const float k = 0.1; //Range of accepted value [u(1-k),u(1+k)]
                         //Store velocity read

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

    if (v > u * (1 + k)) //upper threshold
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

    else if (v < u * (1 - k)) //lower threshold
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
    else if (u == 0) //Stop the motor
        switch (select)
        {
        case 1:
            w1(0, 1);
            break;
        case 2:
            w2(0, 1);
            break;
        case 3:
            w3(0, 1);
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

    float rot_speed;           //rotating speed in rad/s
    const int interval = 1000; //choose interval is 1 second (1000 milliseconds)
    currentMillis = millis();

    if (currentMillis - previousMillis > interval)
    {
        previousMillis = currentMillis;
        rot_speed = (float)((currentEncoder - previousEncoder) * 2 * PI / Encoder_1_round);
        previousEncoder = currentEncoder;
        return rot_speed;
    }
}

void position(float x, float y, float w)
{
    //Control the car to an exact relative position

    const int StError = 0;
    const int scale = 1;

    //wheel speed value
    float u1, u2, u3;
    u1 = ControlMatrix(x, y, w).u1;
    u2 = ControlMatrix(x, y, w).u2;
    u3 = ControlMatrix(x, y, w).u3;

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

    //wheel rotation angle values
    //consider in the same interval of time, the value of rotating angle is equal to
    //the value of rotating speed, considering the same control matrix
    float p_u1, p_u2, p_u3;
    p_u1 = u1;
    p_u2 = u2;
    p_u3 = u3;
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
            oldPosition_1 = newPosition_1;
        //
        //
        if (newPosition_2 != oldPosition_2)
            oldPosition_2 = newPosition_2;
        //
        //
        if (newPosition_3 != oldPosition_3)
            oldPosition_2 = newPosition_2;

        if (!(newPosition_1 < abs(scale * p_u1 * 30000 / (2 * PI)) + StError || newPosition_2 < abs(scale * p_u2 * 30000 / (2 * PI)) + StError || newPosition_3 < abs(scale * p_u3 * 30000 / (2 * PI)) + StError))
            break;

    } while (newPosition_1 < abs(scale * p_u1 * 30000 / (2 * PI)) + StError || newPosition_2 < abs(scale * p_u2 * 30000 / (2 * PI)) + StError || newPosition_3 < abs(scale * p_u3 * 30000 / (2 * PI)) + StError);

    newPosition_1 = newPosition_2 = newPosition_3 = 0;
    Encoder_1.write(0);
    Encoder_2.write(0);
    Encoder_3.write(0);
}

long encoder_output(int select)
{
    //Output encoder value
    switch (select)
    {
    case 1:
        return Encoder_1.read();
        break;
    case 2:
        return Encoder_2.read();
        break;
    case 3:
        return Encoder_3.read();
        break;
    default:
        return 0;
    }
}

void bluetooth_controlPID(void)
{
    command = Serial.read();
    Serial.println(command);

    // Serial.println(command);
    switch (command)
    {
    //Adjust speed
    case '1':
        speedcar = 0.3;
        break;
    case '2':
        speedcar = 0.35;
        break;
    case '3':
        speedcar = 0.4;
        break;
    case '4':
        speedcar = 0.45;
        break;
    case '5':
        speedcar = 0.5;
        break;
    case '6':
        speedcar = 0.6;
        break;
    case '7':
        speedcar = 0.7;
        break;
    case '8':
        speedcar = 0.8;
        break;
    case '9':
        speedcar = 0.9;
        break;
    case '0':
        speedcar = 0;
        break;
    case 'q':
        speedcar = 1;
        break;

    //Control motion
    case 'F': //Forward
        control_PID(0, 1);
        control_PID(-4 * speedcar, 2);
        control_PID(4 * speedcar, 3);
        break;

    case 'B': //Backward
        control_PID(0, 1);
        control_PID(4 * speedcar, 2);
        control_PID(-4 * speedcar, 3);
        break;

    case 'R': //Right
        control_PID(-4 * speedcar, 1);
        control_PID(2 * speedcar, 2);
        control_PID(2 * speedcar, 3);
        break;

    case 'L': //Left
        control_PID(4 * speedcar, +1);
        control_PID(-2 * speedcar, 2);
        control_PID(-2 * speedcar, 3);
        break;

    case 'G': //Forward left
        control_PID(2 * speedcar, 1);
        control_PID(-4 * speedcar, 2);
        control_PID(1 * speedcar, 3);
        break;

    case 'I': //Forward right
        control_PID(-2 * speedcar, 1);
        control_PID(-4 * speedcar, 2);
        control_PID(1 * speedcar, 3);
        break;

    case 'H': //Backward left
        control_PID(2 * speedcar, 1);
        control_PID(1 * speedcar, 2);
        control_PID(-4 * speedcar, 3);
        break;

    case 'J': //Backward right
        control_PID(-2 * speedcar, 1);
        control_PID(4 * speedcar, 2);
        control_PID(-1 * speedcar, 3);
        break;

    case 'W': //Rotate left
        control_PID(4 * speedcar, 1);
        control_PID(4 * speedcar, 2);
        control_PID(4 * speedcar, 3);
        delay(100);
        break;

    case 'w': //Rotate left
        control_PID(4 * speedcar, 1);
        control_PID(4 * speedcar, 2);
        control_PID(4 * speedcar, 3);
        delay(100);
        break;

    case 'U': //Rotate right
        control_PID(-4 * speedcar, 1);
        control_PID(-4 * speedcar, 2);
        control_PID(-4 * speedcar, 3);
        delay(100);
        break;

    case 'u': //Rotate right
        control_PID(-4 * speedcar, 1);
        control_PID(-4 * speedcar, 2);
        control_PID(-4 * speedcar, 3);
        delay(100);
        break;

    case 'S': //No motor input
        w1(0, 1);
        w2(0, 1);
        w3(0, 1);
        break;

    case 'D': //Disconnected
        w1(0, 1);
        w2(0, 1);
        w3(0, 1);
        break;

    case 'X': //Go square
        position(1, 0, 0);
        delay(100);
        position(0, 1, 0);
        delay(100);
        position(-1, 0, 0);
        delay(100);
        position(0, -1, 0);
        delay(100);
        break;

    default:
        break;
    }
}

void bluetooth_control(void)
{
    command = Serial.read();
    Serial.println(command);

    // Serial.println(command);
    switch (command)
    {
    //Adjust speed
    case '1':
        speedcar = 0.3;
        break;
    case '2':
        speedcar = 0.35;
        break;
    case '3':
        speedcar = 0.4;
        break;
    case '4':
        speedcar = 0.45;
        break;
    case '5':
        speedcar = 0.5;
        break;
    case '6':
        speedcar = 0.6;
        break;
    case '7':
        speedcar = 0.7;
        break;
    case '8':
        speedcar = 0.8;
        break;
    case '9':
        speedcar = 0.9;
        break;
    case '0':
        speedcar = 0;
        break;
    case 'q':
        speedcar = 1;
        break;

    //motor control
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

    case 'D':
        w1(0, 1);
        w2(0, 1);
        w3(0, 1);
        break;

    default:
        break;
    }
}