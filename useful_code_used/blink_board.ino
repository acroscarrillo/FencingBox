void setup() {
  // initialize digital pin LED_DEBUG = PCINT17 as an output.
  pinMode(PCINT17, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(PCINT17, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(1000);                      // wait for a second
  digitalWrite(PCINT17, LOW);   // turn the LED off by making the voltage LOW
  delay(1000);                      // wait for a second
}
