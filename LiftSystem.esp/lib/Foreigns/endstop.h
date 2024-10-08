//
// endstop.h -- endstop handler for motor controller for Riks Lift
//
// BSla, 14 jun 2024
//
// 6 Jul 2022  Only one endstop position is kept: the going active position
//
#ifndef _ENDSTOP_H
#define _ENDSTOP_H
#include <Arduino.h>
#include "simulation.h"

class Endstop {
  public:
   Endstop  (const char *_name): name (_name) {}
   ~Endstop () {}
   void setup (uint8_t thisPin);
   void loop ();
   bool reached ();
   void clearEndstopPosition ();
   void setOffset (float offset);
   bool getEndstopPosition (float &pos);
   void checkChanged (); 
  private:
   uint8_t     pin;
   const char *name; 
   bool        active;
   bool        previous;
   float       previousPos;
   bool        goingActiveSeen;
   float       endstopPosition;
   uint32_t    latestTransitionTime;
#if SIMULATION==1
   Simulation *simPtr;
#endif
};

extern Endstop lowerEndstop, upperEndstop;
#endif