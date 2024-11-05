//
// liftencoder.cpp -- readout of digital encoder for Riks lift
//
// BSla, 21 Aug 2023
//
#include "simulation.h"
#include "hsi.h"
#include "liftencoder.h"

#define _DEBUG 0
#include "debug.h"

LiftEncoder liftEncoder;


volatile int32_t LiftEncoder::position = 0;
static uint8_t encA = GPIO_ENC_A;
static uint8_t encB = GPIO_ENC_B;

int32_t LiftEncoder::toIncrements (float meters) 
{
     return int32_t (meters / MeterPerIncrement);
}

float LiftEncoder::toMeters (int32_t increments)
{
    return (float (increments) * MeterPerIncrement); 
}

bool LiftEncoder::isEqual (float p1, float p2)
{
  return (fabs (p1-p2) < MeterPerIncrement);
}




#if SIMULATION == 0

#define MIN_SAME 10 // minimum needed identical samples of AB signal

void ARDUINO_ISR_ATTR encoderISR()
{
   static uint8_t prevVal = 0;
   uint8_t vp = 0;
   uint8_t v = digitalRead(encB) << 1 | digitalRead(encA);
   int sameCount = 0;
   while (sameCount < MIN_SAME)
   {
      v = digitalRead(encB) << 1 | digitalRead(encA);
      if (v == vp)
         sameCount++;
      else
      {
         sameCount = 0;
         vp = v;
      }
   }
   if (v > 1)
      v = 5 - v; // convert gray to bin: 2->3 and 3->2
   int8_t diff = v - prevVal;
   prevVal = v;
   if (diff < -1)
      diff += 4; // range -1..3
   if (diff > 2)
      diff -= 4; // range -1..2
   if (diff == 2)
      diff = 0; // range -1..1
   Liftencoder::position += diff;
}

#endif // simulation == 0

bool LiftEncoder::setup()
{
   DEBUG(">  liftencoder::setup ()\n");
   position = 0;
   previousPosition = 1;
#if SIMULATION == 0
   pinMode(encA, INPUT);
   pinMode(encB, INPUT);

   // setup encA and encB to generate pin change interrupts
   attachInterrupt(encA, encoderISR, CHANGE);
   attachInterrupt(encB, encoderISR, CHANGE);

#endif
   DEBUG("<  liftEncoder:: setup done\n");
   return true;
}

float LiftEncoder::getPosition()
{
  float fpos;
#if SIMULATION == 0
  int32_t ipos;
  noInterrupts ();
  ipos = position;
  interrupts ();
  fpos = toMeters (ipos));
#else 
  fpos = sim.getPosition ();
#endif
  if (!isEqual (fpos, previousPosition)) {
     //DEBUG (">< LiftEncoder::getPosition: pos = %f m \n", fpos);
     previousPosition = fpos;
  }
  return fpos;
}

