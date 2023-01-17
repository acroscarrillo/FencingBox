#include <WS2812.h>  // Super lightweight library to drive this led strips/arrays

WS2812 LED(256);  // There are 256 leds in a 16x16 array (as it is actually a 256 led strip)
cRGB value;       // This stores the rgb value to be used in a led. Dont worry, code is easy.

// 16x16 array of leds to create an image, in this case message "Foil".
// These numbers indicate led position and will get later on assigned a color.
uint8_t foil_symbol_array[] = {
  18,20,21,22,23,24,25,26,27,28,29,34,38,45,50,57,61,66,77,82,85,86,87,93,103,107,109,114,116,120,136,137,138,
  141,146,166,168,169,170,171,173,178,195,196,197,198,199,200,201,202,203,205,210,235,237
};

void setup() {
  LED.setOutput(PCINT20);   // Led array/strip data wire 
  LED.setColorOrderGRB();   // Led color arrangement. Some led drivers use RGB convention...etc.
  displayReset();  //function defined below that turns all leds off
}

void loop() {
// one_led();
foil_logo();   // Method defined bellow to draw message "Foil"
delay(500);
displayReset();
displayHitOnTarB(); // Method defined bellow to signal a hit on target from fencer B
delay(500);
displayReset();
}

// Method to signal a hit on target from fencer B
void one_led() {
  value.b = 0;
  value.g = 1; 
  value.r = 0;       
  LED.set_crgb_at(0, value);                  
  LED.sync(); // Sends the value to the LED
  delay(10); // Wait (ms)
}

// Method to signal a hit on target from fencer B
void displayHitOnTarB() {
  value.b = 0;
  value.g = 1; 
  value.r = 0; 
  for (int i = 0; i < 256/2-16; i++){      
    LED.set_crgb_at(i+144, value);                  
  } 
  LED.sync(); // Sends the value to the LED
  delay(10); // Wait (ms)
}

// Method to signal a hit on target from fencer A
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

// Method to clear display
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

// Method to display text "Foil"
void foil_logo() {
  value.b = 1; value.g = 1; value.r = 1;
  for (int i = 0; i < 55; i++){      
    int index = foil_symbol_array[i];
    LED.set_crgb_at(index, value);
  }          
  LED.sync(); // Sends the value to the LED
  delay(10);
}
