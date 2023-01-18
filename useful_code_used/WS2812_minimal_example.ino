#include <WS2812.h>
#include <avr/interrupt.h>


#define TEST_LIGHTS       // turns on lights for a second on start up
#define REPORT_TIMING     // prints timings over serial interface
#define BUZZERTIME  1000  // length of time the buzzer is kept on after a hit (ms)
#define LIGHTTIME   2000  // length of time the lights are kept on after a hit (ms)
#define BAUDRATE   57600  // baudrate of the serial debug interface


//============
// Pin Setup
//============
// const uint8_t shortLEDA  =  8;    // Short Circuit A Light
// const uint8_t shortLEDB  = 13;    // Short Circuit A Light


const uint8_t groundPinA = A0;    // Ground A pin - Analog
const uint8_t weaponPinA = A1;    // Weapon A pin - Analog
const uint8_t lamePinA   = A2;    // Lame   A pin - Analog (Epee return path)
const uint8_t lamePinB   = A3;    // Lame   B pin - Analog (Epee return path)
const uint8_t weaponPinB = A4;    // Weapon B pin - Analog
const uint8_t groundPinB = A5;    // Ground B pin - Analog

const byte modePin    =  PCINT18;        // Mode change button interrupt pin 0 (digital pin 2)
const byte buzzerPin  =  PCINT16;        // buzzer pin
const byte debugLED  =  PCINT17;        // debug LED

//=========================
// values of analog reads
//=========================
int weaponA = 0;
int weaponB = 0;
int lameA   = 0;
int lameB   = 0;
int groundA = 0;
int groundB = 0;

//=======================
// depress and timeouts
//=======================
long depressAtime = 0;
long depressBtime = 0;
bool lockedOut    = false;

//==========================
// Lockout & Depress Times
//==========================
// the lockout time between hits for foil is 300ms +/-25ms
// the minimum amount of time the tip needs to be depressed for foil 14ms +/-1ms
// the lockout time between hits for epee is 45ms +/-5ms (40ms -> 50ms)
// the minimum amount of time the tip needs to be depressed for epee 2ms
// the lockout time between hits for sabre is 170ms +/-10ms
// the minimum amount of time the tip needs to be depressed (in contact) for sabre 0.1ms -> 1ms
// These values are stored as micro seconds for more accuracy
//                         foil   epee   sabre
const long lockout [] = {300000,  45000, 170000};  // the lockout time between hits
const long depress [] = { 14000,   2000,   1000};  // the minimum amount of time the tip needs to be depressed



//=================
// mode constants
//=================
const uint8_t FOIL_MODE  = 0;
const uint8_t EPEE_MODE  = 1;
const uint8_t SABRE_MODE = 2;

uint8_t currentMode = FOIL_MODE;

volatile bool modeJustChangedFlag = false;

//=========
// states
//=========
bool depressedA  = false;
bool depressedB  = false;
bool hitOnTargA  = false;
bool hitOffTargA = false;
bool hitOnTargB  = false;
bool hitOffTargB = false;

bool lightOnTargA  = false;
bool lightOffTargA = false;
bool lightOnTargB  = false;
bool lightOffTargB = false;


//==============
// Screen Setup
//==============
WS2812 LED(256); 
cRGB value;
#define disp_brtness 5 //Number 0-255 set  to 80 is good

uint8_t foil_symbol_array[] = {
  18,20,21,22,23,24,25,26,27,28,29,34,38,45,50,57,61,66,77,82,85,86,87,93,103,107,109,114,116,120,136,137,138,
  141,146,166,168,169,170,171,173,178,195,196,197,198,199,200,201,202,203,205,210,235,237
};

uint8_t epee_symbol_array[]= {
  2,3,4,5,6,7,8,9,10,11,13,18,20,24,25,29,34,38,39,43,45,50,52,61,77,80,81,82,83,84,85,86,87,88,
  103,107,109,114,116,117,118,119,120,141,146,148,149,150,151,152,167,169,171,173,178,180,182,183,184,205,
  210,212,213,214,215,216,231,233,235,237,242,244,246,247,248
};

uint8_t sabre_symbol_array[]= {
  3,4,5,10,13,18,20,25,29,34,39,43,45,50,53,54,55,60,77,82,84,85,86,87,88,103,105,107,109,114,116,117,118,120,
  141,146,148,149,150,151,152,153,154,155,168,171,173,178,180,181,182,183,205,210,212,213,214,215,216,232,237,
  242,246,247
};

uint8_t gc_wings_array[] = {
4,5,22,23,25,26,28,29,35,36,38,39,40,42,52,54,55,58,59,70,71,73,74,76,83,84,85,87,168,170,171,172,179,181,182,184,185,196,197,200,201,203,213,215,216,217,219,220,226,227,229,230,232,233,250,251
};

uint8_t gc_torch_array[] = {
65,67,90,91,92,93,94,96,98,100,101,102,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,126,129,131,132,133,134,135,136,137,138,139,140,141,142,143,144,145,153,154,155,157,159,161,162,163,164,165,188,190
};

//================
// Configuration
//================
void setup() {
  delay(500);
  LED.setOutput(PCINT20); 
  LED.setColorOrderGRB();  
   // set the internal pullup resistor on modePin
   pinMode(modePin, INPUT_PULLUP);

   // add the interrupt to the mode pin (interrupt is pin 0)
   attachInterrupt(modePin-2, changeMode, FALLING);

   pinMode(buzzerPin,  OUTPUT);

#ifdef TEST_LIGHTS
   testLights();
#endif

    setModeLeds();
    resetValues();
}


//============
// Main Loop
//============
void loop() {
   // use a while as a main loop as the loop() has too much overhead for fast analogReads
   // we get a 3-4% speed up on the loop this way
   while(1) {
      checkIfModeChanged();
      // read analog pins
      weaponA = analogRead(weaponPinA);
      weaponB = analogRead(weaponPinB);
      lameA   = analogRead(lamePinA);
      lameB   = analogRead(lamePinB);
      
      if      (currentMode == FOIL_MODE)
         foil();
      else if (currentMode == EPEE_MODE)
         epee();
      else if (currentMode == SABRE_MODE)
         sabre();

      signalHits();
   }
}


//=====================
// Mode pin interrupt
//=====================
void changeMode() {
   // set a flag to keep the time in the ISR to a min
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  // If interrupts come faster than 200ms, assume it's a bounce and ignore
  if (interrupt_time - last_interrupt_time > 200) 
  {
   modeJustChangedFlag = true;
  }
  last_interrupt_time = interrupt_time;
}



//============================
// Sets the correct mode led
//============================
void setModeLeds() {
   if (currentMode == FOIL_MODE) {
      foil_logo();
   }
    else {
      if (currentMode == EPEE_MODE) {
        epee_logo();
      } else {
         if (currentMode == SABRE_MODE){
            sabre_logo();
         }
      }
   }
}


//========================
// Run when mode changed
//========================
void checkIfModeChanged() {
  if (modeJustChangedFlag) {
    if (currentMode == 2)
      currentMode = 0;
    else
      currentMode++;
      
    setModeLeds();
    resetValues();

    modeJustChangedFlag = false;
   }
}


//===================
// Main foil method
//===================
void foil() {

   long now = micros();
   if (((hitOnTargA || hitOffTargA) && (depressAtime + lockout[FOIL_MODE] < now)) || 
       ((hitOnTargB || hitOffTargB) && (depressBtime + lockout[FOIL_MODE] < now))) {
      lockedOut = true;
   }

   // weapon A
   if (hitOnTargA == false && hitOffTargA == false) { // ignore if A has already hit
      // off target
      if (900 < weaponA && lameB < 100) {
         if (!depressedA) {
            depressAtime = micros();
            depressedA   = true;
         } else {
            if (depressAtime + depress[FOIL_MODE] <= micros()) {
               hitOffTargA = true;
            }
         }
      } else {
      // on target
         if (400 < weaponA && weaponA < 600 && 400 < lameB && lameB < 600) {
            if (!depressedA) {
               depressAtime = micros();
               depressedA   = true;
            } else {
               if (depressAtime + depress[FOIL_MODE] <= micros()) {
                  hitOnTargA = true;
               }
            }
         } else {
            // reset these values if the depress time is short.
            depressAtime = 0;
            depressedA   = 0;
         }
      }
   }

   // weapon B
   if (hitOnTargB == false && hitOffTargB == false) { // ignore if B has already hit
      // off target
      if (900 < weaponB && lameA < 100) {
         if (!depressedB) {
            depressBtime = micros();
            depressedB   = true;
         } else {
            if (depressBtime + depress[FOIL_MODE] <= micros()) {
               hitOffTargB = true;
            }
         }
      } else {
      // on target
         if (400 < weaponB && weaponB < 600 && 400 < lameA && lameA < 600) {
            if (!depressedB) {
               depressBtime = micros();
               depressedB   = true;
            } else {
               if (depressBtime + depress[FOIL_MODE] <= micros()) {
                  hitOnTargB = true;
               }
            }
         } else {
            // reset these values if the depress time is short.
            depressBtime = 0;
            depressedB   = 0;
         }
      }
   }
}


//===================
// Main epee method
//===================
void epee() {
   long now = micros();
   if ((hitOnTargA && (depressAtime + lockout[1] < now)) || (hitOnTargB && (depressBtime + lockout[1] < now))) {
      lockedOut = true;
   }

   // weapon A
   //  no hit for A yet    && weapon depress    && opponent lame touched
   if (hitOnTargA == false) {
      if (400 < weaponA && weaponA < 600 && 400 < lameA && lameA < 600) {
         if (!depressedA) {
            depressAtime = micros();
            depressedA   = true;
         } else {
            if (depressAtime + depress[1] <= micros()) {
               hitOnTargA = true;
            }
         }
      } else {
         // reset these values if the depress time is short.
         if (depressedA == true) {
            depressAtime = 0;
            depressedA   = 0;
         }
      }
   }

   // weapon B
   //  no hit for B yet    && weapon depress    && opponent lame touched
   if (hitOnTargB == false) {
      if (400 < weaponB && weaponB < 600 && 400 < lameB && lameB < 600) {
         if (!depressedB) {
            depressBtime = micros();
            depressedB   = true;
         } else {
            if (depressBtime + depress[1] <= micros()) {
               hitOnTargB = true;
            }
         }
      } else {
         // reset these values if the depress time is short.
         if (depressedB == true) {
            depressBtime = 0;
            depressedB   = 0;
         }
      }
   }
}


//===================
// Main sabre method
//===================
void sabre() {

   long now = micros();
   if (((hitOnTargA || hitOffTargA) && (depressAtime + lockout[2] < now)) || 
       ((hitOnTargB || hitOffTargB) && (depressBtime + lockout[2] < now))) {
      lockedOut = true;
   }

   // weapon A
   if (hitOnTargA == false && hitOffTargA == false) { // ignore if A has already hit
      // on target
      if (400 < weaponA && weaponA < 600 && 400 < lameB && lameB < 600) {
         if (!depressedA) {
            depressAtime = micros();
            depressedA   = true;
         } else {
            if (depressAtime + depress[2] <= micros()) {
               hitOnTargA = true;
            }
         }
      } else {
         // reset these values if the depress time is short.
         depressAtime = 0;
         depressedA   = 0;
      }
   }

   // weapon B
   if (hitOnTargB == false && hitOffTargB == false) { // ignore if B has already hit
      // on target
      if (400 < weaponB && weaponB < 600 && 400 < lameA && lameA < 600) {
         if (!depressedB) {
            depressBtime = micros();
            depressedB   = true;
         } else {
            if (depressBtime + depress[2] <= micros()) {
               hitOnTargB = true;
            }
         }
      } else {
         // reset these values if the depress time is short.
         depressBtime = 0;
         depressedB   = 0;
      }
   }
}


//==============
// Signal Hits
//==============
void signalHits() {
   // non time critical, this is run after a hit has been detected
   if (lockedOut) {
      resetValues();
   }

  if(hitOnTargA && (lightOnTargA == false)){
    // DISPLAY HIT ON TARGET A
    displayHitOnTarA();
    lightOnTargA = true;
  }
  if(hitOffTargA && (lightOffTargA == false)){
    // DISPLAY HIT OFF TARGET A
    displayHitOffTarA();
    lightOffTargA = true;
  }
  if(hitOffTargB && (lightOffTargB == false)){
    // DISPLAY HIT OFF TARGET B
    displayHitOffTarB();
    lightOffTargB = true;
  }
  if(hitOnTargB && (lightOnTargB == false)){
    // DISPLAY HIT ON TARGET B
    displayHitOnTarB();
    lightOnTargB = true;
  }
}


//======================
// Reset all variables
//======================
void resetValues() {
   delay(BUZZERTIME);             // wait before turning off the buzzer
   digitalWrite(buzzerPin,  LOW);
   delay(LIGHTTIME-BUZZERTIME);   // wait before turning off the lights

   displayReset();

   lockedOut    = false;
   depressAtime = 0;
   depressedA   = false;
   depressBtime = 0;
   depressedB   = false;

   hitOnTargA  = false;
   hitOffTargA = false;
   hitOnTargB  = false;
   hitOffTargB = false;

   lightOnTargA  = false;
   lightOffTargA = false;
   lightOnTargB  = false;
   lightOffTargB = false;

   delay(100);
}


//==============
// Test lights
//==============
void testLights() {
  value.b = disp_brtness; value.g = disp_brtness; value.r = disp_brtness;
  for (int i = 0; i < 56; i++){      
    int index1 = gc_wings_array[i];
    LED.set_crgb_at(index1, value);
    int index2 = gc_torch_array[i];
    LED.set_crgb_at(index2, value);
  }     

  LED.sync(); // Sends the value to the LED
  digitalWrite(buzzerPin,HIGH);
  delay(50); // Wait (ms)
  digitalWrite(buzzerPin,LOW);
  delay(50);
  digitalWrite(buzzerPin,HIGH);
  delay(50); // Wait (ms)
  digitalWrite(buzzerPin,LOW);
  delay(50); // Wait (ms)
  digitalWrite(buzzerPin,HIGH);
  delay(500); // Wait (ms)
  digitalWrite(buzzerPin,LOW);
  delay(1300);
  displayReset();
}

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


void displayHitOffTarA() {
  digitalWrite(buzzerPin,  HIGH);
  value.b = disp_brtness;
  value.g = disp_brtness; 
  value.r = disp_brtness; 
  for (int i = 0; i < 256/2-16; i++){      
    LED.set_crgb_at(i, value);                  
  } 
  LED.sync(); // Sends the value to the LED
  delay(10); // Wait (ms)
}


void displayHitOnTarA() {
  digitalWrite(buzzerPin,  HIGH);
  value.b = 0;
  value.g = 0; 
  value.r = disp_brtness; 
  for (int i = 0; i < 256/2-16; i++){      
    LED.set_crgb_at(i, value);                  
  } 
  LED.sync(); // Sends the value to the LED
  delay(10); // Wait (ms)
}


void displayHitOnTarB() {
  digitalWrite(buzzerPin,  HIGH);
  value.b = 0;
  value.g = disp_brtness; 
  value.r = 0; 
  for (int i = 0; i < 256/2-16; i++){      
    LED.set_crgb_at(i+144, value);                  
  } 
  LED.sync(); // Sends the value to the LED
  delay(10); // Wait (ms)
}


void displayHitOffTarB() {
  digitalWrite(buzzerPin,  HIGH);
  value.b = disp_brtness;
  value.g = disp_brtness; 
  value.r = disp_brtness; 
  for (int i = 0; i < 256/2-16; i++){      
    LED.set_crgb_at(i+144, value);                  
  } 
  LED.sync(); // Sends the value to the LED
  delay(10); // Wait (ms)
}

void foil_logo() {
  digitalWrite(buzzerPin,  HIGH);
  value.b = disp_brtness; value.g = disp_brtness; value.r = disp_brtness;
  for (int i = 0; i < 55; i++){      
    int index = foil_symbol_array[i];
    LED.set_crgb_at(index, value);
  }          
  LED.sync(); // Sends the value to the LED
  delay(10);
}

void epee_logo() {
  digitalWrite(buzzerPin,  HIGH);
  value.b = disp_brtness; value.g = disp_brtness; value.r = disp_brtness;
  for (int i = 0; i < 81; i++){      
    int index = epee_symbol_array[i];
    LED.set_crgb_at(index, value);
  }          
  LED.sync(); // Sends the value to the LED
  delay(10);
}

void sabre_logo() {
  digitalWrite(buzzerPin,  HIGH);
  value.b = disp_brtness; value.g = disp_brtness; value.r = disp_brtness;
  for (int i = 0; i < 64; i++){      
    int index = sabre_symbol_array[i];
    LED.set_crgb_at(index, value);
  }          
  LED.sync(); // Sends the value to the LED
  delay(10);
}
