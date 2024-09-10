//
// endstop.cpp -- endstops for Riks Lift
//
// BSla, 14 jun 2024
//  6 jul 2024 "The" endstop position is the going active position 
#include <arduino.h>
#include "liftencoder.h"
#include "endstop.h"

#define _DEBUG 1
#include "debug.h"


#define MINIMUM_INT_TIME    200   // milliseconds between valid interrupts (for debounce)

Endstop lowerEndstop ("lowerEndstop"), upperEndstop ("upperEndstop");   // the only instances of Endstop that exist


#if SIMULATION==0
void ARDUINO_ISR_ATTR endstopISR ();
#endif

void Endstop::setup (uint8_t thisPin)
// setup this instance of endstop. Set up from liftPosition
//  
{
   DEBUG (">  %s:: setup (pin = %d)\n", name, thisPin);  
   clearEndstopPosition ();
   pin = thisPin;
   pinMode (pin, INPUT);
   active = false;
   latestTransitionTime = millis ();
#if SIMULATION==0
   attachInterrupt (pin, endstopISR, CHANGE);
   active = digitalRead (pin);
#else
   simPtr = sim.getSimPtr (); 
#endif
   DEBUG ("<  %s (pin %d) done\n", name, thisPin);
}

#if SIMULATION==0
void ARDUINO_ISR_ATTR endstopISR ()
// pin change interrupt handler
{
   upperEndstop.checkChanged ();
   lowerEndstop.checkChanged ();
}
#endif

void Endstop::checkChanged ()
// check if an endstop changed state and if so, record it, and
// the position at which it occurred.
// Called from both interrupt context and loop () context. 
// Typically, changes are always discovered in the interrupt context
{
   uint32_t now = millis ();
   bool expired;
#if SIMULATION==0
   // real mode
   active = digitalRead (pin);
   expired = (now - latestTransitionTime > MINIMUM_INT_TIME);
#else // simulation
   if (this == &upperEndstop) {
      active = simPtr -> isAboveUpper ();
   }
   else { // lower endstop
      active = simPtr -> isBelowLower ();
   }
   expired = true;
#endif
   if ((active != previous) && expired) {
      // a transition beyond debounce
      previous = active;
      latestTransitionTime = now;

      float pos = LiftEncoder::getPosition ();

      if (active) {
         goingActiveSeen = true;
         endstopPosition = pos;
      }   
   }
}

void Endstop::loop () 
{
   checkChanged ();     // pickup stray missed interrupts
}

bool Endstop::reached ()
{
   return active;
}

void Endstop::clearEndstopPosition ()
// prepare for new endstop position measurement
{
   DEBUG (">  %s:: clearEndstopPositions ()\n", name);
#if SIMULATION==0
   noInterrupts ();
#endif
   goingActiveSeen = false;
   endstopPosition = 0.0;
#if SIMULATION==0
   interrupts ();
   DEBUG ("<  %s:: clearEndstopPositions\n", name);
#endif
}

void Endstop::setOffset (float offset)
// add offset to switch positions
{
  DEBUG (">< %s::setOffset, offset = %f\n", name, offset);
  endstopPosition   += offset;
}

bool Endstop::getEndstopPosition (float &position)
// get position at which endstop switched on
{
   bool success = goingActiveSeen;
   if (success) {
      position = endstopPosition; 
   }
   if (success) {
      DEBUG (">< %s::getEndstopPosition: pos = %f [m]\n", name, position);
   }
   else {
      DEBUG (">< %s::getEndstopPositions goingActiveSeen = %s, *** FAILED\n", name, toCCP (goingActiveSeen));
   }
   return success;
}
