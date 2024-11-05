//
// liftencoder.h -- class definition for RiksLift motor controller
// in Riks Lift, encA = GPIO32, pin 6 right, encB = GPIO33, pin 5 right
//
// BSla, 14 jun 2024
//
#ifndef _LIFTENCODER_H
#define _LIFTENCODER_H

#include <Arduino.h>

class LiftEncoder
{
public:
   static constexpr float MeterPerIncrement = 0.000346008;
   LiftEncoder() {}
   ~LiftEncoder() {}
   bool setup();
   float getPosition();
   void setPosition(float position);
   void setOffset(float offset);

   static float toMeters (int32_t increments);
   static int32_t toIncrements (float meters);
   static bool isEqual (float p1, float p2);


   static volatile int32_t position;  // used in interrupt context too

private:
   float previousPosition;

};

extern LiftEncoder liftEncoder;
#endif