#ifndef Omni_h
#define Omni_h

void w1(int rotation, int direct);
void w2(int rotation, int direct);
void w3(int rotation, int direct);
void Plot(float x, float y, float w);
int sign_of(float x);
float read_speed(int select);
void control_ONOFF(float u, int select);
void control_PID(float u, int select);

#endif
