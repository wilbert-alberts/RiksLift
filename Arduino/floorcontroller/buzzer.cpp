//
// buzzer.cpp -- buzzer control
//
// BSla, 7 Jul 2024
//
#include "hsi.h"
#include "timer.h"
#include "buzzer.h"

#define _DEBUG 0
#include "debug.h"

#define BUZZ_PERIOD        80 MILLISECONDS

Buzzer buzzer;


void Buzzer::setup ()
{
   DEBUG (">< Buzzer: setup\n");
   pinMode (GPIO_BUZZER, OUTPUT);
   buzzing = false;
   switchBuzzer ();
   isOn = false;
   buzzTimer.start (BUZZ_PERIOD);
}

void Buzzer::loop ()
{
   if (isOn && buzzTimer.hasExpired ()) {
      buzzing = !buzzing;
      switchBuzzer ();
      buzzTimer.restart ();
   }
}

void Buzzer::buzz (bool on) 
{
   buzzing = isOn = on;
   switchBuzzer ();
   buzzTimer.start ();
}
