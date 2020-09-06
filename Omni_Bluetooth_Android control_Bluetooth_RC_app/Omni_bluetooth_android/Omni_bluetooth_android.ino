#define IN1A 22
#define IN2A 23
#define IN1B 24
#define IN2B 25
#define IN1C 26
#define IN2C 27
#define EN1 11
#define EN2 12
#define EN3 13
void w1(int rotation, int direct);
void w2(int rotation, int direct);
void w3(int rotation, int direct);

int error = 0;
byte type = 0;
byte vibrate = 0;
double x, y, w;
int u1_sign, u2_sign, u3_sign;
float in_min, in_max, out_min, out_max;
float M_in_min, M_in_max, M_out_min, M_out_max;

int speedcar = 0;
char command;

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
    //initial check motor on start up
    w1(255, 1);
    w2(255, 1);
    w3(255, 1);
    delay(100);
    w1(0, 1);
    w2(0, 1);
    w3(0, 1);
    delay(100);
    w1(255, -1);
    w2(255, -1);
    w3(255, -1);
    delay(100);
    w1(0, 1);
    w2(0, 1);
    w3(0, 1);
}

void loop()
{
    if (Serial.available() == 3)
    {
        x = Serial.read();
        y = Serial.read();
        w = Serial.read();
    }

    Serial.println(x);
    Serial.println(y);
    Serial.println(w);
    Serial.println("********");
    //remap value
    in_min = 0;
    in_max = 255;
    out_min = -1;
    out_max = 1;

    //remapping joy stick's value
    x = (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    y = (y - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;

    //Deadzone
    if (x <= 0.5 && x >= -0.5)
        x = 0;
    if (y <= 0.5 && y >= -0.5)
        y = 0;

    // rotating
    if (w == 0)
    {
        w = -5.72;
        x = 0;
        y = 0;
    }
    else if (w == 255)
    {
        w = 5.72;
        x = 0;
        y = 0;
    }
    else
    {
        w = 0;
    }

    //Define Omni wheel robot parameter
    /*
float r=0.175;
float l=0.053;
*/
    //motor value
    float u1 = (175 * w - 1000 * x) / 53;
    float u2 = (175 * w + 500 * x - 500 * sqrt(3) * y) / 53;
    float u3 = (175 * w + 500 * x + 500 * sqrt(3) * y) / 53;

    /*
 float u1 = (l * w - x) / r;
 float u2 = (2*l * w + x - sqrt(3) * y) / 2*r;
 float u3 = (2*l * w + x + sqrt(3) * y) / 2*r;
*/

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

    u1 = abs(u1);
    u2 = abs(u2);
    u3 = abs(u3);

    //Mapping value
    M_in_min = 0;
    //M_in_max = 19;
    M_in_max = max(max(u1, u2), u3);
    M_out_min = 0;
    M_out_max = 255;

    //Remapping motors' value
    u1 = (u1 - M_in_min) * (M_out_max - M_out_min) / (M_in_max - in_min) + M_out_min;
    u2 = (u2 - M_in_min) * (M_out_max - M_out_min) / (M_in_max - in_min) + M_out_min;
    u3 = (u3 - M_in_min) * (M_out_max - M_out_min) / (M_in_max - in_min) + M_out_min;

    w1(u1, u1_sign);
    w2(u2, u2_sign);
    w3(u3, u3_sign);
    
    
    Serial.print("u1: ");
    Serial.print(u1);
    Serial.print("  u1-sign: ");
    Serial.print(u1_sign);
    Serial.println("");

    Serial.print("u2: ");
    Serial.print(u2);
    Serial.print("  u2-sign: ");
    Serial.print(u2_sign);
    Serial.println("");

    Serial.print("u3: ");
    Serial.print(u3);
    Serial.print("  u3-sign: ");
    Serial.print(u3_sign);
    Serial.println("");
    Serial.println("**********");
    
    
    
    


    
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
