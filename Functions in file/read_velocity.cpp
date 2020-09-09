float vel(int select) 
{
    //read velocity of selected motor
    //return velocity in rad/s
    switch (select)
    {
    case 1:
        CurrentEncoder = Encoder_1.read();
        break;
    case 2:
        CurrentEncoder = Encoder_2.read();
        break;
    case 3:
        CurrentEncoder = Encoder_3.read();
        break;
    }

    currentMillis = millis();
    if (currentMillis - previousMillis > interval)
    {
        previousMillis = currentMillis;

        rpm = (float)abs(((CurrentEncoder - previousEncoder) * 2 * PI / ENCODEROUTPUT));
        Serial.println(rpm);
        previousEncoder = CurrentEncoder;

        return rpm;
    }
}


