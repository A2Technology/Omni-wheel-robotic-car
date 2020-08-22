#include <PS2X_lib.h> //for v1.6
#define IN1A 22
#define IN2A 23
#define IN1B 24
#define IN2B 25
#define IN1C 26
#define IN2C 27
#define EN1 11
#define EN2 12
#define EN3 13
/******************************************************************
 * set pins connected to PS2 controller:
 *   - 1e column: original 
 *   - 2e colmun: Stef?
 * replace pin numbers by the ones you use
 ******************************************************************/
#define PS2_DAT 10 //MOSI
#define PS2_CMD 9  //MISO
#define PS2_SEL 29 //SS
#define PS2_CLK 28 //CLK

/******************************************************************
 * select modes of PS2 controller:
 *   - pressures = analog reading of push-butttons 
 *   - rumble    = motor rumbling
 * uncomment 1 of the lines for each mode selection
 ******************************************************************/
//#define pressures   true
#define pressures false
//#define rumble      true
#define rumble false

PS2X ps2x; // create PS2 Controller Class

//right now, the library does NOT support hot pluggable controllers, meaning
//you must always either restart your Arduino after you connect the controller,
//or call config_gamepad(pins) again after connecting the controller.

int error = 0;
byte type = 0;
byte vibrate = 0;
double x, y, w;
int u1_sign, u2_sign, u3_sign;
float in_min, in_max, out_min, out_max;
float M_in_min, M_in_max, M_out_min, M_out_max;

int speedcar = 0;
char command;

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

void setup()
{

    pinMode(IN1A, OUTPUT);
    pinMode(IN2A, OUTPUT);
    pinMode(IN1B, OUTPUT);
    pinMode(IN2B, OUTPUT);
    pinMode(IN1C, OUTPUT);
    pinMode(IN2C, OUTPUT);

    Serial.begin(57600);
    delay(300); //added delay to give wireless ps2 module some time to startup, before configuring it

    //CHANGES for v1.6 HERE!!! **************PAY ATTENTION*************

    //setup pins and settings: GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error
    error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);

    if (error == 0)
    {
        Serial.print("Found Controller, configured successful ");
        Serial.print("pressures = ");
        if (pressures)
            Serial.println("true ");
        else
            Serial.println("false");
        Serial.print("rumble = ");
        if (rumble)
            Serial.println("true)");
        else
            Serial.println("false");
        Serial.println("Try out all the buttons, X will vibrate the controller, faster as you press harder;");
        Serial.println("holding L1 or R1 will print out the analog stick values.");
        Serial.println("Note: Go to www.billporter.info for updates and to report bugs.");
    }
    else if (error == 1)
        Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");

    else if (error == 2)
        Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");

    else if (error == 3)
        Serial.println("Controller refusing to enter Pressures mode, may not support it. ");

    //  Serial.print(ps2x.Analog(1), HEX);

    type = ps2x.readType();
    switch (type)
    {
    case 0:
        Serial.print("Unknown Controller type found ");
        break;
    case 1:
        Serial.print("DualShock Controller found ");
        break;
    case 2:
        Serial.print("GuitarHero Controller found ");
        break;
    case 3:
        Serial.print("Wireless Sony DualShock Controller found ");
        break;
    }
}

void loop()
{

    if (error == 1) //skip loop if no controller found
        return;

    else
    {                                      //DualShock Controller
        ps2x.read_gamepad(false, vibrate); //read controller and set large motor to spin at 'vibrate' speed

        y = ps2x.Analog(PSS_LY); // read the x and Y value of the left joy stick
        x = 255 - ps2x.Analog(PSS_LX);

        //re map value
        in_min = 0;
        in_max = 255;
        out_min = -1;
        out_max = 1;

        y = (y - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
        x = (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;

        // rotating
        if (ps2x.Button(PSB_L1))
        {
            w = -5.72;
            x = 0;
            y = 0;
        }
        else if (ps2x.Button(PSB_R1))
        {
            w = 5.72;
            x = 0;
            y = 0;
        }
        else
        {
            w = 0;
        }
    }
    //motor value
    float u1 = (175 * w - 1000 * x) / 53;
    float u2 = (175 * w + 500 * x - 500 * sqrt(3) * y) / 53;
    float u3 = (175 * w + 500 * x + 500 * sqrt(3) * y) / 53;

    M_in_min = 0;
    M_in_max = 18.9;
    M_out_min = 0u;
    M_out_max = 255;

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

    u1 = (abs(u1) - M_in_min) * (M_out_max - M_out_min) / (M_in_max - in_min) + M_out_min;
    u2 = (abs(u2) - M_in_min) * (M_out_max - M_out_min) / (M_in_max - in_min) + M_out_min;
    u3 = (abs(u3) - M_in_min) * (M_out_max - M_out_min) / (M_in_max - in_min) + M_out_min;

    w1(u1, -u1_sign);
    w2(u2, u2_sign);
    w3(u3, u3_sign);

    Serial.print("Motor Values:");
    Serial.print(u1);
    Serial.print(",\n");
    Serial.print(u2);
    Serial.print(",\n");
    Serial.print(u3);
    Serial.print(",\n");
    Serial.print("Motor Direction:");
    Serial.print(u1_sign);
    Serial.print(",");
    Serial.print(u2_sign);
    Serial.print(",");
    Serial.print(u3_sign);
    Serial.print(",");
}
