#include <Encoder.h>
#define IN1A 22
#define IN2A 23
#define IN1B 24
#define IN2B 25
#define IN1C 26
#define IN2C 27
#define EN1 11
#define EN2 12
#define EN3 13
#define EN_const 30000
Encoder Encoder_1(2, 3);
Encoder Encoder_2(18, 19);
Encoder Encoder_3(20, 21);

long oldPosition_1 = 0;
long newPosition_1;
long oldPosition_2 = 0;
long newPosition_2;
long oldPosition_3 = 0;
long newPosition_3;

float x, y, w;
int u1_sign, u2_sign, u3_sign;
float in_min, in_max, out_min, out_max;
float M_in_min, M_in_max, M_out_min, M_out_max;
int speedcar = 0;
const int scale = 1;
const int StError = 5;
void Plot(float x, float y, float w);
void w1(int rotation, int direct);
void w2(int rotation, int direct);
void reset_encoder(void);

void setup()
{
    pinMode(IN1A, OUTPUT);
    pinMode(IN2A, OUTPUT);
    pinMode(IN1B, OUTPUT);
    pinMode(IN2B, OUTPUT);
    pinMode(IN1C, OUTPUT);
    pinMode(IN2C, OUTPUT);
    Serial.begin(9600);
}

void loop()
{
    reset_encoder;
    Plot(0.5, 0, 0);
    Serial.print("Encoder 1: ");
    Serial.println(newPosition_1);
    Serial.print("Encoder 2: ");
    Serial.println(newPosition_2);
    Serial.print("Encoder 3: ");
    Serial.println(newPosition_3);



//
//    reset_encoder;
//    Plot(0, 0.5, 0);
//    reset_encoder;
//    Plot(0, -0.5, 0);

    //
    //        Plot(-0.5, 0, 0);
    //        delay(20000000);
    //
    //        Plot(0, -0.5, 0);
    //        delay(20000000);
}

void w1(int rotation, int direct)
{
    analogWrite(EN1, rotation);
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
    analogWrite(EN2, rotation);
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
    analogWrite(EN3, rotation);
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

void Plot(float x, float y, float w)
{
    //motor value
    float u1 = (175 * w - 1000 * x) / 53;
    float u2 = (175 * w + 500 * x - 500 * sqrt(3) * y) / 53;
    float u3 = (175 * w + 500 * x + 500 * sqrt(3) * y) / 53;
    if (u1 >= 0)
        u1_sign = 1;
    else
        u1_sign = -1;
    if (u2 >= 0)
        u2_sign = 1;
    else
        u2_sign = -1;
    if (u3 >= 0)
        u3_sign = 1;
    else
        u3_sign = -1;

    //Taking absolute values
    u1 = abs(u1);
    u2 = abs(u2);
    u3 = abs(u3);

    //Mapping value
    M_in_min = 0;
    M_in_max = max(max(u1, u2), u3);
    M_out_min = 0;
    M_out_max = 255;

    //Remapping motors' value
    u1 = (u1 - M_in_min) * (M_out_max - M_out_min) / (M_in_max - in_min) + M_out_min;
    u2 = (u2 - M_in_min) * (M_out_max - M_out_min) / (M_in_max - in_min) + M_out_min;
    u3 = (u3 - M_in_min) * (M_out_max - M_out_min) / (M_in_max - in_min) + M_out_min;

    //Position matrix ( x y in meter, w in rad, p1 p2 p3 in rad)
    float p1 = (175 * w - 1000 * x) / 53;
    float p2 = (175 * w + 500 * x - 500 * sqrt(3) * y) / 53;
    float p3 = (175 * w + 500 * x + 500 * sqrt(3) * y) / 53;
    //Map p1 p2 p3 to encoder value. 1 round of encoder is EN_const=30000
    p1 = p1 * EN_const / (2 * PI);
    p2 = p2 * EN_const / (2 * PI);
    p3 = p3 * EN_const / (2 * PI);
//    Serial.println(p1);
//    Serial.println(p2);
//    Serial.println(p3);
   
    //Encoder read
    long newPosition_1 = abs(Encoder_1.read());
    if (newPosition_1 != oldPosition_1)
    {
        oldPosition_1 = newPosition_1;
        Serial.print("Encoder 1: ");
        Serial.println(newPosition_1);
    }

    long newPosition_2 = abs(Encoder_2.read());
    if (newPosition_2 != oldPosition_2)
    {
        oldPosition_2 = newPosition_2;
        Serial.print("Encoder 2: ");
        Serial.println(newPosition_2);
    }

    long newPosition_3 = abs(Encoder_3.read());
    if (newPosition_3 != oldPosition_3)
    {
        oldPosition_3 = newPosition_3;
        Serial.print("Encoder 3: ");
        Serial.println(newPosition_3);
    }

    ////Encoder value constraints
    //Wheel 1
    
        if (abs(newPosition_1) < abs(p1 + StError))
            w1(u1, u1_sign);
        else
            w1(0, 0);

        //Wheel 2
        if (abs(newPosition_2) < abs(p2 + StError))
            w2(u2, u2_sign);
        else
            w2(0, 0);

        //Wheel 3
        if (abs(newPosition_3) < abs(p3 + StError))
            w3(u3, u3_sign);
        else
            w3(0, 0);
    
}
void reset_encoder(void)
{
    Encoder_1.write(0);
    Encoder_2.write(0);
    Encoder_3.write(0);
}
