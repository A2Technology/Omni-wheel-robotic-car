#define Encoder1A 2
#define Encoder2A 3

int encoderValue=0;

void count(void); // code for counting the increasing values of encoder ticks 
void setup()
{
  Serial.begin(9600);
  pinMode(19,INPUT);
  attachInterrupt(digitalPinToInterrupt(18),count,FALLING);
  encoderValue=0;
}
void loop()
{
  Serial.print("Starting\n");
  // to print encoder value on the screen 
  Serial.print("Encoder Value="); 
  Serial.println(encoderValue);
}

void count()
{
encoderValue++;
}
