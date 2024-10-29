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

static volatile int32_t position = 0;
static      uint8_t encA     = GPIO_ENC_A;
static      uint8_t encB     = GPIO_ENC_B;
int32_t     LiftEncoder::previousPosition;


#if SIMULATION==0

#define MIN_SAME 10   // minimum needed identical samples of AB signal

void ARDUINO_ISR_ATTR encoderISR ()
{
   static uint8_t prevVal = 0;
   uint8_t vp = 0;
   uint8_t v = digitalRead (encB) << 1 | digitalRead (encA);
   int sameCount = 0;
   while (sameCount < MIN_SAME) {
      v = digitalRead (encB) << 1 | digitalRead (encA);
      if (v == vp) sameCount++; 
      else {
        sameCount = 0;
        vp = v;
      }
   }
   if (v > 1) v = 5-v;  // convert gray to bin: 2->3 and 3->2
   int8_t diff = v - prevVal;
   prevVal = v;
   if (diff < -1) diff += 4;  // range -1..3
   if (diff > 2) diff -= 4;   // range -1..2
   if (diff == 2) diff = 0;   // range -1..1
   position = position + diff;
}

#endif // simulation == 0

bool LiftEncoder::setup ()
{
    DEBUG (">  liftencoder::setup ()"\n)
    position = 0;
    previousPosition = 1;
#if SIMULATION==0
    pinMode (encA, INPUT);
    pinMode (encB, INPUT);

    // setup encA and encB to generate pin change interrupts 
    attachInterrupt (encA, encoderISR, CHANGE);
    attachInterrupt (encB, encoderISR, CHANGE);

#endif
    DEBUG ("<  liftEncoder:: setup done\n")
    return true;
}

int32_t LiftEncoder::getIncPosition ()
{
    int32_t pos;
#if SIMULATION==0  
    noInterrupts ();   
    pos = position; 
    interrupts ();
#else
    pos = toIncrements (sim.getPosition ());
#endif
    if (!isEqual (pos,  previousPosition)) {
       DEBUG (">< LiftEncoder::getIncPosition: pos = %d inc, %f m \n", pos, toMeters (pos));
       previousPosition = pos;
    }
    return pos;
}

float LiftEncoder::getPosition () 
{
  float fpos;
#if SIMULATION == 0
  fpos = toMeters (getIncPosition ());
#else 
  fpos = sim.getPosition ();
#endif
  return fpos;
}


// void LiftEncoder::setOffset (float offset)
// {
// #if SIMULATION == 0  //    noInterrupts ();   
//    position += toIncrements (offset);
//    interrupts ();
// #else
//    sim.setOffset (offset));
// #endif
// }
