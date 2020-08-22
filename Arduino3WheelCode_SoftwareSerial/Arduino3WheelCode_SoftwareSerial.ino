#include <SoftwareSerial.h>

SoftwareSerial mySerial(A2, A3);  // Connect the TXD pin of BT module to pin A2 of the Arduino and the RXD pin of BT module to pin A3 of Arduino.

signed int x;
signed int y;
signed int z;
signed int w;

float a;
float m;

String str;
char dir = 0;
int start_val = 0;

int Mf = 2;  
int Ml = 4; 
int Mb = 7;

int Ef = 3; 
int El = 9; 
int Eb = 10; 

int e1 = 0;
int e2 = 0;
int e3 = 0;

int e1p = 0;
int e2p = 0;
int e3p = 0;

int kicker = 0;

int i = 0;
int counter = 0;

boolean fast = true;
boolean StateA0 = false;
boolean StateA1 = false;
boolean State11 = false;

char getData;

double set_speed = 1;
double speed_val = 1;

void process();
void turnright();
void turnleft();

void setup() {
  
pinMode(Mf, OUTPUT);    //direction of forward motor (located on upper right position)
pinMode(Ml, OUTPUT);    //direction of left motor (located on upper left position)
pinMode(Mb, OUTPUT);    //direction of back motor (located on bottom position)
pinMode(Ef, OUTPUT);    //speed of forward motor
pinMode(El, OUTPUT);    //speed of left motor
pinMode(Eb, OUTPUT);    //speed of back motor
pinMode(A0, INPUT);
pinMode(A1, INPUT);
pinMode(6, OUTPUT);
pinMode(11, OUTPUT);
pinMode(12, OUTPUT);
pinMode(13, OUTPUT);

  mySerial.begin(9600);
  mySerial.println("Start");
  Serial.begin(9600);
  Serial.println("Start");
    
  for(i = 0; i < 10; i++) {
    speed_val = analogRead(A0);
    speed_val = speed_val / 1023 * 5;
  }
  
  digitalWrite(6, LOW); 
}

void test()  // for communication testing purpose
{
  if (digitalRead(A1) == LOW) 
  {
    counter++;
    if (counter > 10) counter = 0;
    digitalWrite(12, HIGH);
    mySerial.println("Success (" + String(counter) + ")");
    delay(500);
  }
  else digitalWrite(12, LOW);  
}

void loop() 
{
  //test();  //test communication (optional)
    
    while(mySerial.available())
    {
      
     //test();  //test communication (optional)
     
      char getData = mySerial.read();
      
        if (getData == 'M')
        {
          m = mySerial.parseFloat();
          
          if (mySerial.read() == '#') 
          {
            processM();
          }
        }
        
        if (getData == 'A')
        {
          a = mySerial.parseFloat();
          
          if (mySerial.read() == '#') 
          {
            processA();
          }
        }
       
        if (getData == 'a')
        {  
          delay(5);        
          if (mySerial.read() == '#') 
          {
            turnright();
          }
        }      
       
        if (getData == 'b')
        {    
          delay(5);
          if (mySerial.read() == '#') 
          {
            stoprobot();
          }
        }   

        if (getData == 'c')
        {     
          delay(5);
          if (mySerial.read() == '#') 
          {
            turnleft();
          }
        } 

        if (getData == 'x')
        {  
          delay(5);        
          if (mySerial.read() == '#') 
          {
            processx();
          }
        }      
       
        if (getData == 'y')
        {    
          delay(5);
          if (mySerial.read() == '#') 
          {
            processy();
          }
        }   

        if (getData == 'z')
        {     
          delay(5);
          if (mySerial.read() == '#') 
          {
            processz();
          }
        } 
        
        if (getData == '~') {
        
          x = mySerial.parseInt();
        
            if (mySerial.read() == '*') {
        
              y = mySerial.parseInt();
        
                if (mySerial.read() == '@') {
                
                  z = mySerial.parseInt();
                  
                    if (mySerial.read() == '#') {
                     
                       process();

                    }                
                } 
            }
        }       
    }
    
}

void processa(){              //button a pressed
  mySerial.println("Button a! ");
  turnright();
  mySerial.flush();  
}

void processb(){              //button b pressed
  mySerial.println("Button b! "); 
  stoprobot();
  mySerial.flush(); 
}

void processc(){              //button c pressed
  mySerial.println("Button c! ");
  turnleft();
  mySerial.flush();  
}

void processx(){              //button x pressed
  mySerial.println("Button x! "); 
  mySerial.flush(); 
  fast = true;
}

void processy(){              //button y pressed
  mySerial.println("Button y! "); 
  mySerial.flush(); 
}

void processz(){              //button z pressed
  mySerial.println("Button z! "); 
  mySerial.flush(); 
  fast = false;
}

void processM(){              //button Magnitude pressed
  mySerial.print("Received Magnitude: ");
  mySerial.println(m); 
  mySerial.flush();   
}

void processA(){              //button Angle pressed
  mySerial.print("Received Angle: ");
  mySerial.println(a); 
  mySerial.flush();   
}

void turnright() {
  
  mySerial.println("TURNING RIGHT");
  
  digitalWrite(Ml, HIGH);
  digitalWrite(Mf, HIGH);
  digitalWrite(Mb, HIGH);

  speed_val = analogRead(A0);
  speed_val = speed_val / 1023 * 5;
  
  e1p = 100 * speed_val;
  e2p = 100 * speed_val;  
  e3p = 100 * speed_val;
  
  analogWrite(El, e2p);  
  analogWrite(Eb, e3p); 
  analogWrite(Ef, e1p); 
}

void turnleft() {
  
  mySerial.println("TURNING LEFT");
  
  digitalWrite(Ml, LOW);
  digitalWrite(Mf, LOW);
  digitalWrite(Mb, LOW);

  speed_val = analogRead(A0);
  speed_val = speed_val / 1023 * 5;
  
  e1p = 100 * speed_val;
  e2p = 100 * speed_val;  
  e3p = 100 * speed_val;
  
  analogWrite(El, e2p);  
  analogWrite(Eb, e3p); 
  analogWrite(Ef, e1p); 
}
  
void stoprobot() {
  
  mySerial.println("STOP");
  
  digitalWrite(Ml, LOW);
  digitalWrite(Mf, LOW);
  digitalWrite(Mb, LOW);

  speed_val = analogRead(A0);
  speed_val = speed_val / 1023 * 5;
  
  e1p = 0 * speed_val;
  e2p = 0 * speed_val;  
  e3p = 0 * speed_val;
  
  analogWrite(El, e2p);  
  analogWrite(Eb, e3p); 
  analogWrite(Ef, e1p); 
}

void process() {

  speed_val = analogRead(A0);           //robot's overall speed control
  speed_val = speed_val / 1023 * 5;
      
//  Serial.print("Received x: ");
//  Serial.print(x);              
//  Serial.print("      Received y: ");
//  Serial.print(y);       
//  Serial.print("         Received z: ");
//  Serial.print(z);       
//  Serial.print("            Received w: ");
//  Serial.println(w); 
  

//  x = constrain(x, 0, 255);
//  y = constrain(y, 0, 255);
//  z = constrain(z, 0, 255);
  
  e1 = map(x, 0, 450, 0, 255);
//  mySerial.print("Motor 1: ");
///  mySerial.print(e1); 
  if (e1 < 0) { digitalWrite(Mf, LOW); //mySerial.print("  M1 Low"); 
  } 
  else if (e1 >= 0)  { digitalWrite(Mf, HIGH); //mySerial.print("  M1 High"); 
  } 
  e1p = abs(e1);

  
  e2 = map(y, 0, 450, 0, 255);
//  mySerial.print("Motor 2: ");
///  mySerial.print(e2); 
  if (e2 < 0) { digitalWrite(Ml, LOW); //mySerial.print("  M2 Low"); 
  } 
  else if (e2 >= 0) { digitalWrite(Ml, HIGH); //mySerial.print("  M2 High"); 
  } 
  e2p = abs(e2);

  
  e3 = map(z, 0, 450, 0, 255);
//  mySerial.print("Motor 3: ");
///  mySerial.print(e3); 
  if (e3 < 0) { digitalWrite(Mb, LOW); //mySerial.println("  M3 Low"); 
  } 
  else if (e3 >= 0)  { digitalWrite(Mb, HIGH); //mySerial.println("  M3 High"); 
  } 
  
  e3p = abs(e3);
  
  e1p = e1p * speed_val;
  e2p = e2p * speed_val;  
  e3p = e3p * speed_val;
  
  analogWrite(El, e2p);  
  analogWrite(Eb, e3p); 
  analogWrite(Ef, e1p);    
  
  
  mySerial.flush(); 
}  
