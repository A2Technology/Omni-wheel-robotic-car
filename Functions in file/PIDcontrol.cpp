double kp = 20, ki = 10, kd = 0.01, input_1 = 0, output_1 = 0, setpoint_1 = 0, input_2 = 0, output_2 = 0, setpoint_2 = 0, input_3 = 0, output_3 = 0, setpoint_3 = 0;
PID PID_1(&input_1, &output_1, &setpoint_1, kp, ki, kd, DIRECT);
PID PID_2(&input_2, &output_2, &setpoint_2, kp, ki, kd, DIRECT);
PID PID_3(&input_3, &output_3, &setpoint_3, kp, ki, kd, DIRECT);

void control_PID(float u, int select)
{
    //Get sign of rotating velocity of wheels
    int u_sign = sign_of(u);

    switch (select)
    {
    case 1:
        setpoint_1 = u;
        input_1 = vel(1);
        PID_1.Compute();
        w1(output_1, u_sign);
        break;

    case 2:
        setpoint_2 = u;
        input_2 = vel(2);
        PID_2.Compute();
        w2(output_2, u_sign);
        break;

    case 3:
        setpoint_3 = u;
        input_3 = vel(3);
        PID_3.Compute();
        w3(output_3, u_sign);
        break;
    }
}