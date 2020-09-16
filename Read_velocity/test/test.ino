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
//Declare encoder functions
Encoder Encoder_1(EN_1A, EN_1B);
Encoder Encoder_2(EN_2A, EN_2B);
Encoder Encoder_3(EN_3A, EN_3B);
float speedd=0;
//Time variables
long previousMillis = 0;
long currentMillis = 0;

//Encoder variables
volatile long currentEncoder;
volatile long previousEncoder;
volatile long oldPosition_1 = 0;
volatile long oldPosition_2 = 0;
volatile long oldPosition_3 = 0;
volatile long newPosition_1;
volatile long newPosition_2;
volatile long newPosition_3;
float v = 0; 
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

float read_speed(int select)
{
    //read velocity of selected motor
    //return velocity in rad/s
    const int Encoder_1_round = 30000; //define number of pulses in one round of encoder
    switch (select)
    {
    case 1:
        currentEncoder = -Encoder_1.read();
        break;
    case 2:
        currentEncoder = -Encoder_2.read();
        break;
    case 3:
        currentEncoder = -Encoder_3.read();
        break;
    }

    float rot_speed;           //rotating speed in rad/s
    const int interval = 1000; //choose interval is 1 second (1000 milliseconds)
    currentMillis = millis();

    if (currentMillis - previousMillis > interval)
    {
        previousMillis = currentMillis;
        rot_speed = (float)abs(((currentEncoder - previousEncoder) * 2 * PI / Encoder_1_round));
        previousEncoder = currentEncoder;
        return rot_speed;
    }
}

void setup()
{
  // put your setup code here, to run once:
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
  pinMode(EN_1A, INPUT_PULLUP);
  pinMode(EN_1B,  INPUT_PULLUP);
  pinMode(EN_2A,  INPUT_PULLUP);
  pinMode(EN_2B,  INPUT_PULLUP);
  pinMode(EN_3A,  INPUT_PULLUP);
  pinMode(EN_3B,  INPUT_PULLUP);


  //initial check motor on start up
  w1(255, 1);
  w2(255, 1);
  w3(255, 1);
  delay(200);
  w1(0, 1);
  w2(0, 1);
  w3(0, 1);
  delay(600);
  w1(255, -1);
  w2(255, -1);
  w3(255, -1);
  delay(200);
  w1(0, 1);
  w2(0, 1);
  w3(0, 1);
}

void loop()
{
  w2(255,1);
  speedd = read_speed(2);
  if (speedd!=0)
  {
    Serial.println(speedd,5);
  }
  else if (speedd ==0)
    Serial.println("no input");

  //bluetooth_control();
}
