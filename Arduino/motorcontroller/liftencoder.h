//
// liftencoder.h -- class definition for RiksLift motor controller
// in Riks Lift, encA = GPIO32, pin 6 right, encB = GPIO33, pin 5 right
//
// BSla, 14 jun 2024
//
#ifndef _LIFTENCODER_H
#define _LIFTENCODER_H

#include <Arduino.h>

class LiftEncoder {
  public:
   LiftEncoder () {}
   ~LiftEncoder () {}
   bool setup ();
   static float getPosition ();
   static int32_t getIncPosition ();
   static void setPosition (float position);
   static void setPosition (int32_t position);
   static void setOffset (float offset);
   static void setOffset (int32_t offset);
   static float toMeters (int32_t increments);
   static int32_t toIncrements (float meters);
  private:
   static int32_t previousPosition;
#if SIMULATION==1
   static Simulation *simPtr;
#endif
};

extern LiftEncoder liftEncoder;
#endif