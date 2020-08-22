#define IN1A 22
#define IN2A 23
#define IN1B 24
#define IN2B 25
#define IN1C 26
#define IN2C 27
#define EN1  11
#define EN2  12
#define EN3  13


int speedcar=0;
char command;

void w1(int rotation, int direct)
{
  analogWrite(EN1, rotation);
  if (direct==1)
  {
  digitalWrite(IN1A, HIGH);
  digitalWrite(IN2A, LOW);
  }
  else if (direct==-1)
  {
  digitalWrite(IN1A, LOW);
  digitalWrite(IN2A, HIGH);
  }
}

void w2(int rotation, int direct)
{
  analogWrite(EN2, rotation);
  if (direct==1)
  {
  digitalWrite(IN1B, HIGH);
  digitalWrite(IN2B, LOW);
  }
  else if (direct==-1)
  {
  digitalWrite(IN1B, LOW);
  digitalWrite(IN2B, HIGH);
  }
}

void w3(int rotation, int direct)
{
  analogWrite(EN3, rotation);
  if (direct==1)
  {
  digitalWrite(IN1C, HIGH);
  digitalWrite(IN2C, LOW);
  }
  else if (direct==-1)
  {
  digitalWrite(IN1C, LOW);
  digitalWrite(IN2C, HIGH);
  }
}

void setup() {
  // put your setup code here, to run once:
    Serial.begin(9600);  //Set the band rate to your Bluetooth module.
    pinMode(IN1A, OUTPUT);
    pinMode(IN2A, OUTPUT);
    pinMode(IN1B, OUTPUT);
    pinMode(IN2B, OUTPUT);
    pinMode(IN1C, OUTPUT);
    pinMode(IN2C, OUTPUT);
}
void loop()
{
 command = Serial.read();
    switch(command)
    {
      case '0': speedcar=0; break; 
      case '1': speedcar=25; break; 
      case '2': speedcar=51; break; 
      case '3': speedcar=76; break; 
      case '4': speedcar=102; break; 
      case '5': speedcar=127; break; 
      case '6': speedcar=153; break; 
      case '7': speedcar=178; break; 
      case '8': speedcar=204; break; 
      case '9': speedcar=230; break;
      case 'q': speedcar=255; break;  
  
    }
     //initialize with motors stoped
    
      

    Serial.println(command);
    switch(command)
    {
    case 'F':  
      w1(0,-1); 
      w2(255,1);
      w3(255,-1);
      break;
      
    case 'B':  
      w1(0,+1); 
      w2(255,-1);
      w3(255,1);
      break;
      
    case 'R'://right
      w1(255,-1); 
      w2(180,-1);
      w3(180,-1);
      break;
      
    case 'L'://left
      w1(255,+1); 
      w2(180,+1);
      w3(180,+1);
      break;
    
    case 'S':
      w1(0,+1); 
      w2(0,+1);
      w3(0,-1);
     break;
   }

   
}
 
