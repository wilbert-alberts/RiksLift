//
// pushbutton.h -- methods for pushbutton with LEDs
//
// BSla, 7 Jul 2024
//
#ifndef _PUSHBUTTON_H
#define _PUSHBUTTON_H

#include <Arduino.h>
#include "hsi.h"
#include "Timer.h"

#include "liftglobals.h"

#define DEBOUNCE_TIME (50 MILLISECONDS)

class Pushbutton {
  public:
   Pushbutton (uint8_t theButton, uint32_t debounceTime, const char *_name):
        simPushed (false), followPushed (true), wasPushed (true), pbGPIO (theButton), 
        publishTimer (REPORT_INTERVAL), debounceTimer (debounceTime), name (_name)
   {}
   virtual ~Pushbutton () {}
   void setup ();
   void loop ();
   void report (bool pushed);
   bool isPushed ();
   void setSimPushed (bool s) {simPushed = s;}
  private:
   bool      simPushed;
   bool      followPushed;
   bool      wasPushed;
   uint8_t   pbGPIO;
   Timer     publishTimer;
   Timer     debounceTimer;
   const char *name;
   bool      isPushedBouncing ();
};

struct led {
   uint8_t gpio;
   bool isOn;
   bool isFlashing;
};

class LitPushbutton:public Pushbutton {
  public:
   enum LedColor {
     Red, Green, Blue, None // ids are used as indexes
   };

   LitPushbutton (uint8_t theBtn, uint8_t lRed, uint8_t lGreen, uint8_t lBlue, const char *_name);
   virtual  ~LitPushbutton () {}
   void setup ();
   void loop ();
   void setLED  (LedColor led, bool flashing);
   void setLED (LiftState ls);
   void alarm (bool on);

  private:
   int alarmCount = 0;             // 0 = no alarm
   bool wasAlarming = false;
   LedColor ledColor;              // when no alarm 
   bool isFlashing = false;        // when no alarm
   Timer flashTimer;
   led rgb [3];
   void writeLED (LedColor ledColor, bool flashing);
};

extern LitPushbutton pushbutton;
extern Pushbutton emo;
extern Pushbutton screen;
#endif