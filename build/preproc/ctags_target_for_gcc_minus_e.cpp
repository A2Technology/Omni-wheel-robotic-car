# 1 "d:\\Github\\Omni-wheel-robotic-car\\MainSketch.ino"
# 2 "d:\\Github\\Omni-wheel-robotic-car\\MainSketch.ino" 2

# 2 "d:\\Github\\Omni-wheel-robotic-car\\MainSketch.ino"
int led = 13;

// the setup routine runs once when you press reset:
void setup() {
  // initialize the digital pin as an output.
  pinMode(led, 0x1);
}

// the loop routine runs over and over again forever:
void loop() {
  digitalWrite(led, 0x1); // turn the LED on (HIGH is the voltage level)
  delay(1000); // wait for a second
  digitalWrite(led, 0x0); // turn the LED off by making the voltage LOW
  delay(1000); // wait for a second
}
