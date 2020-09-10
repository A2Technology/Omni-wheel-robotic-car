long interval = 1000; //choose interval is 1 second (1000 milliseconds)
long previousMillis = 0;
long currentMillis = 0;

long currentEncoder;
long previousEncoder;

float read_speed(int select)
{
    //read velocity of selected motor
    //return velocity in rad/s
    const int Encoder_1_round=30 000; //define number of pulses in one round of encoder 
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
    currentMillis = millis();
    if (currentMillis - previousMillis > interval)
    {
        previousMillis = currentMillis;
        rot_speed = (float)abs(((currentEncoder - previousEncoder) * 2 * PI / Encoder_1_round));
        previousEncoder = currentEncoder;
        return rot_speed;
    }
}