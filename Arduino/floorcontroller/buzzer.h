//
// buzzer.h -- methods for buzzer
//
// BSla, 7 Jul 2024
//
#ifndef _BUZZER_H
#define _BUZZER_H

#include <Arduino.h>
#include "timer.h"
#include "hsi.h"

class Buzzer {
  public:
   Buzzer () {}
   ~Buzzer () {}

   void setup ();
   void loop ();
   void buzz (bool on);

  private:
   bool isOn;
   bool buzzing;
   Timer buzzTimer;
   void switchBuzzer () {digitalWrite (GPIO_BUZZER, buzzing);}
};

extern Buzzer buzzer;
#endif