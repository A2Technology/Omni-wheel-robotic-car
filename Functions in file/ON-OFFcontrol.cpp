void control(float u, int select)
{
    //control motor speed using ON-OFF control
    float k = 0.1; //Range of accepted value [u(1-k),u(1+k)]
    float v = 0;   //velocity read

    switch (select) //Select motor
    {
    case 1:
        v = vel(1);
        break;
    case 2:
        v = vel(2);
        break;
    case 3:
        v = vel(3);
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
