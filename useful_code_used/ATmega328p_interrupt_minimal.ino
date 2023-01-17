#include <avr/interrupt.h>

const byte ledPin = PCINT17; //LED_DEBUG pin 
const byte interruptPin = PCINT18; //BTN pin 
volatile byte state = LOW;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(0, blink, FALLING);
}

void loop() {
  digitalWrite(ledPin, state);
}

void blink() {
{
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  // If interrupts come faster than 200ms, assume it's a bounce and ignore
  if (interrupt_time - last_interrupt_time > 200) 
  {
   state=!state;
  }
  last_interrupt_time = interrupt_time;
}}
