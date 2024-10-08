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
   float getPosition ();
   int32_t getIncPosition ();
   void setPosition (float position);
   void setPosition (int32_t position);
   void setOffset (float offset);
   void setOffset (int32_t offset);
   static float toMeters (int32_t increments);
   static int32_t toIncrements (float meters);
  private:
   static int32_t previousPosition;

};

extern LiftEncoder liftEncoder;
#endif