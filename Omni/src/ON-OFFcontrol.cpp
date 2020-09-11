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

Encoder Encoder_1(EN_1A, EN_1B);
Encoder Encoder_2(EN_2A, EN_2B);
Encoder Encoder_3(EN_3A, EN_3B);
void Plot(float x, float y, float w);
void w1(int rotation, int direct);
void w2(int rotation, int direct);
void w3(int rotation, int direct);
int sign_of(float x);
float read_speed(int select);
void control_ONOFF(float u, int select);

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

long urrentEncoder;
long previousEncoder;

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
