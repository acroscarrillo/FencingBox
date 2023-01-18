#include <avr/interrupt.h>
#include <WS2812.h>  // Super lightweight library to drive this led strips/arrays

WS2812 LED(256);  // There are 256 leds in a 16x16 array (as it is actually a 256 led strip)
cRGB value;       // This stores the rgb value to be used in a led. Dont worry, code is easy.

const byte ledPin = PCINT17;
const byte interruptPin = PCINT18;
volatile byte state = LOW;

void setup() {
  LED.setOutput(PCINT20);   // Led array/strip data wire 
  LED.setColorOrderGRB();   // Led color arrangement. Some led drivers use RGB convention...etc.
  displayReset();  //function defined below that turns all leds off
  pinMode(interruptPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  attachInterrupt(0, blink, FALLING);
}

void loop() {
  if (state){
    displayHitOnTarA();
    digitalWrite(ledPin, HIGH);
  }
  else{
    displayReset();
    digitalWrite(ledPin, LOW);
  }
}

void blink() {
{
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  // If interrupts come faster than 100ms, assume it's a bounce and ignore
  if (interrupt_time - last_interrupt_time > 100) 
  {
   state=!state;
  }
  last_interrupt_time = interrupt_time;
}}

void displayReset() {
  value.b = 0;
  value.g = 0; 
  value.r = 0; 
  for (int i = 0; i < 256; i++){      
    LED.set_crgb_at(i, value);                  
  } 
  LED.sync(); // Sends the value to the LED
  delay(10); // Wait (ms)
}

void displayHitOnTarA() {
  value.b = 0;
  value.g = 0; 
  value.r = 1; 
  for (int i = 0; i < 256/2-16; i++){      
    LED.set_crgb_at(i, value);                  
  } 
  LED.sync(); // Sends the value to the LED
  delay(10); // Wait (ms)
}
