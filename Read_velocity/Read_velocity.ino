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
#define ENCODEROUTPUT 30000

Encoder Encoder_1(EN_1A, EN_1B);
Encoder Encoder_2(EN_2A, EN_2B);
Encoder Encoder_3(EN_3A, EN_3B);
void Plot(float x, float y, float w);
void w1(int rotation, int direct);
void w2(int rotation, int direct);
void w3(int rotation, int direct);
int sign_of(float x);
void vel(long encoderValue1, long encoderValue2, long encoderValue3);

long oldPosition_1 = 0;
long newPosition_1;
long oldPosition_2 = 0;
long newPosition_2;
long oldPosition_3 = 0;
long newPosition_3;

long interval = 1000000;
long previousMillis = 0;
long currentMillis = 0;

long rpm_1 = 0;
long rpm_2 = 0;
long rpm_3 = 0;

boolean measureRpm = false;
int motorPwm = 0;
volatile long encoderValue = 0;

void setup()
{
  pinMode(IN1A, OUTPUT);
  pinMode(IN2A, OUTPUT);
  pinMode(IN1B, OUTPUT);
  pinMode(IN2B, OUTPUT);
  pinMode(IN1C, OUTPUT);
  pinMode(IN2C, OUTPUT);
  pinMode(PWM1, OUTPUT);
  pinMode(PWM2, OUTPUT);
  pinMode(PWM3, OUTPUT);
  Serial.begin(9600);
}

void loop()
{
w1(127, 1);
  vel(Encoder_1.read(), Encoder_2.read(), Encoder_3.read());

  
}

void vel(long encoderValue1, long encoderValue2, long encoderValue3)
{

  currentMillis = micros();
  if (currentMillis - previousMillis > interval)
  {
    previousMillis = currentMillis;

    rpm_1 = (float)abs((encoderValue1 * 60 / ENCODEROUTPUT));
    rpm_2 = (float)abs((encoderValue2 * 60 / ENCODEROUTPUT));
    rpm_3 = (float)abs((encoderValue3 * 60 / ENCODEROUTPUT));

    //      Serial.print(encoderValue);
    //      Serial.print(" pulse / ");
    //      Serial.print(ENCODEROUTPUT);
    //      Serial.print(" pulse per rotation x 60 seconds = ");
//    Serial.print(rpm_1);
//    Serial.println(" RPM");

    Encoder_1.write(0);
    Encoder_2.write(0);
    Encoder_3.write(0);
    encoderValue1 = encoderValue2 = encoderValue3 = 0;
  }
}

void w1(int rotation, int direct)
{
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

int sign_of(float x)
{
  if (x >= 0)
    return 1;
  else
    return -1;
}
