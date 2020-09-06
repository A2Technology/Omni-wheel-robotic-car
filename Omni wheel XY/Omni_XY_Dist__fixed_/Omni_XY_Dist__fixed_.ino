#include <Encoder.h>
Encoder Encoder_1(2, 3);
Encoder Encoder_2(18, 19);
Encoder Encoder_3(20, 21);

long oldPosition_1=0; 
long newPosition_1;
long oldPosition_2=0;
long newPosition_2;
long oldPosition_3=0;
long newPosition_3;

#define IN1A 22
#define IN2A 23
#define IN1B 24
#define IN2B 25
#define IN1C 26
#define IN2C 27
#define EN1 11
#define EN2 12
#define EN3 13




double x, y, w;
int u1_sign, u2_sign, u3_sign;
                                                  //float in_min, in_max, out_min, out_max;
                                                  //float M_in_min, M_in_max, M_out_min, M_out_max;
int speedcar = 0;
char command;
int scale = 1;
int StError = 0;





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
      {
          u1_sign = 1;
      }
    else
      {
          u1_sign = -1;
      }
    if (u2 >= 0)
      {
          u2_sign = 1;
      }
    else
      {
          u2_sign = -1;
      }
    if (u3 >= 0)
      {
          u3_sign = 1;
      }
    else
      {
          u3_sign = -1;
      }

u1=abs(u1);
u2=abs(u2);
u3=abs(u3);

//Mapping value
    float M_in_min = 0;
    //M_in_max = 19; 
    float M_in_max = max(max(u1,u2),u3);
    float M_out_min = 0;
    float M_out_max = 255;

//Remapping motors' value
    u1 = (u1 - M_in_min) * (M_out_max - M_out_min) / (M_in_max - M_in_min) + M_out_min;
    u2 = (u2 - M_in_min) * (M_out_max - M_out_min) / (M_in_max - M_in_min) + M_out_min;
    u3 = (u3 - M_in_min) * (M_out_max - M_out_min) / (M_in_max - M_in_min) + M_out_min;
    
    float p_u1 = (175 * w - 1000 * x) / 53;
    float p_u2 = (175 * w + 500 * x - 500 * sqrt(3) * y) / 53;
    float p_u3 = (175 * w + 500 * x + 500 * sqrt(3) * y) / 53;
    
// long newPosition_1 = abs(Encoder_1.read());
// long newPosition_2 = abs(Encoder_2.read());
// long newPosition_3 = abs(Encoder_3.read());

// reading the encoder value
//  if (newPosition_1 != oldPosition_1){oldPosition_1 = newPosition_1;
//                                      Serial.print("Encoder 1: ");
//                                      Serial.println(newPosition_1);}
//  
//  
//  if (newPosition_2 != oldPosition_2){oldPosition_2 = newPosition_2;
//                                      Serial.print("Encoder 2: ");
//                                      Serial.println(newPosition_2);}
//  
// 
//  if (newPosition_3 != oldPosition_3){oldPosition_3 = newPosition_3;
//                                      Serial.print("Encoder 3: ");
//                                      Serial.println(newPosition_3);}

////Wheel 1
//if (newPosition_1 < abs(scale*p_u1*30000/(2*pi))+StError) 
//  w1(u1,u1_sign);
//  else 
//w1(0,0);
//
////Wheel 2
//if (newPosition_2 < abs(scale*p_u2*30000/(2*pi))+StError)
//      w2(u2, u2_sign);
//  else 
//w2(0,0);
//
////Wheel 3
//if (newPosition_3 < abs(scale*p_u3*30000/(2*pi))+StError)
//      w3(u3, u3_sign);
//  else 
//w3(0,0);



   


do
  {

    long newPosition_1 = abs(Encoder_1.read());
    long newPosition_2 = abs(Encoder_2.read());
    long newPosition_3 = abs(Encoder_3.read());

if (newPosition_1 < abs(scale*p_u1*30000/(2*PI))+StError)  w1(u1,u1_sign);       
           else  w1(0,0);

if (newPosition_2 < abs(scale*p_u2*30000/(2*PI))+StError) w2(u2,u2_sign);     
           else  w2(0,0);

if (newPosition_3 < abs(scale*p_u3*30000/(2*PI))+StError) w3(u3,u3_sign);        
           else  w3(0,0);


 if (newPosition_1 != oldPosition_1){oldPosition_1 = newPosition_1;
                                      Serial.print("Encoder 1: ");
                                      Serial.println(newPosition_1);}
//  
//  
  if (newPosition_2 != oldPosition_2){oldPosition_2 = newPosition_2;
                                      Serial.print("Encoder 2: ");
                                      Serial.println(newPosition_2);}
//  
// 
  if (newPosition_3 != oldPosition_3){oldPosition_3 = newPosition_3;
                                      Serial.print("Encoder 3: ");
                                      Serial.println(newPosition_3);}
    
//if (newPosition_1 < abs(scale*p_u1*30000/(2*PI))+StError) 
           //w1(u1,u1_sign);   
           
//if (newPosition_2 < abs(scale*p_u2*30000/(2*PI))+StError)
           //w2(u2, u2_sign);

//if (newPosition_3 < abs(scale*p_u3*30000/(2*PI))+StError)
           //w3(u3, u3_sign);

if (!(newPosition_1 < abs(scale*p_u1*30000/(2*PI))+StError || newPosition_2 < abs(scale*p_u2*30000/(2*PI))+StError || newPosition_3 < abs(scale*p_u3*30000/(2*PI))+StError))
break;
   
  } while (newPosition_1 < abs(scale*p_u1*30000/(2*PI))+StError || newPosition_2 < abs(scale*p_u2*30000/(2*PI))+StError || newPosition_3 < abs(scale*p_u3*30000/(2*PI))+StError);


newPosition_1=newPosition_2=newPosition_3=0;
Encoder_1.write(0);
Encoder_2.write(0);
Encoder_3.write(0);

Serial.print("Encoder 1: ");
                                      Serial.println(newPosition_1);
                                      Serial.print("Encoder 2: ");
                                      Serial.println(newPosition_2);
                                      Serial.print("Encoder 3: ");
                                      Serial.println(newPosition_3);
    
}



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



Plot(1 , 0 ,0);
delay(100);
Plot(0 , 1 ,0);
delay(100);
Plot(-1 , 0 ,0);
delay(100);
Plot(0 , -1 ,0);
delay(100);



}
