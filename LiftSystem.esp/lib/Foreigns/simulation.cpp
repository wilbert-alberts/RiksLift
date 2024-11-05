//
// simulation.cpp -- implementation of simulation class
//
// BSla, 28 jun 2024
//
#include "simulation.h"

#define _DEBUG 1
#include "debug.h"
#include "liftencoder.h"

#if SIMULATION == 1

// total stroke of lift is 2.73 meter
// These are absolute positions
const float UEPos = -1.00; // total of 2.43 m between the home sensors
const float LEPos = UEPos + 2.563;
const float backlash = 0.01;
const float upperHomeDistance = 0.03;
const float lowerHomeDistance = 0.13;
const float maxSpeed = 0.1; // [m/s]
const float maxHz = 92.0;
const float dispPerPeriod = maxSpeed / maxHz;

Simulation sim;

float Simulation::toMeterPerMS(uint8_t Hz)
{
   return (float(Hz) * dispPerPeriod / 1000.0);
}

void Simulation::setToUpperEndstop()
{
   float oldPosition = position;
   position = UEPos - 0.03;
   DEBUG(">< Simulation::setToUpperEndstop: position was %f, is now %f [m]\n", oldPosition, position);
}

void Simulation::setToLowerEndstop()
{
   float oldPosition = position;
   position = LEPos + 0.11;
   DEBUG(">< Simulation::setToLowerEndstop: position was %f, is now %f [m]\n", oldPosition, position);
}

void Simulation::setup(bool initPosition)
{
   DEBUG(">  Simulation::setup (initPosition\n");
   if (initPosition || !initEverDone)
   {
      initEverDone = true;
      position = 0.0;
   }
   emoActive = false;
   offset = 0.0;
   previousPosition = 5.0;
   speed = 0; // current pwm value
   meterPerMS = 0.0;
   previousMillis = millis ();
   movingForward = movingReverse = previousForward = previousReverse = false;
   oldAboveUpper = oldBelowLower = false;
   speedWarningGiven = posWarningGiven = false;
   //reportTimer.start(1 SECOND);

   DEBUG("<  Simulation::setup done\n");
}

void Simulation::loop()
{
   loopCount++;
   if (movingForward || movingReverse)
   {
      if (speed == 0)
      {
         if (!speedWarningGiven)
         {
            speedWarningGiven = true;
            DEBUG("**** Sim: loop %ld: moving, but speed == 0\n", loopCount);
         }
      }
      else
         speedWarningGiven = false;


      uint32_t now = millis();
      uint32_t exp = now - previousMillis;

      if (!emoActive)
      { // simulate EMO power off
         position += meterPerMS * exp;
      }

      // DEBUG ("Sim: loop %ld: position = %f m\n", loopCount,
      //         position);
      const float range = 4.0;
      if (position > range)
      {
         position = range;
         if (!posWarningGiven)
         {
            DEBUG("**** Sim: position > %f meter\n", range);
            posWarningGiven = true;
         }
      }
      else if (position < -range)
      {
         position = -range;
         if (!posWarningGiven)
         {
            DEBUG("**** Sim: position < %f meter\n", position);
            posWarningGiven = true;
         }
      }
      else if (posWarningGiven)
      {
         DEBUG("   Sim: position = %f is back in range\n", position);
         posWarningGiven = false;
      }
   }
   previousMillis = millis ();
}

bool Simulation::isBelowPhysicalLower()
{
   return (position > (LEPos + lowerHomeDistance)) ? true : false;
}

bool Simulation::isAbovePhysicalUpper()
{
   return (position < (UEPos - upperHomeDistance)) ? true : false;
}

bool Simulation::isAboveUpperGoingUp()
{
   return (position < UEPos) ? true : false;
}

bool Simulation::isAboveUpperGoingDown()
{
   return (position < (UEPos + backlash)) ? true : false;
}

bool Simulation::isBelowLowerGoingUp()
{
   return (position > (LEPos - backlash)) ? true : false;
}

bool Simulation::isBelowLowerGoingDown()
{
   return (position > LEPos) ? true : false;
}

bool Simulation::isAboveUpper()
{
   // DEBUG (">  Simulation::isAboveUpper () \n");
   bool b = (movingForward ? isAboveUpperGoingDown() : isAboveUpperGoingUp());
   if (b != oldAboveUpper)
   {
      oldAboveUpper = b;
      DEBUG("   Simulation: isAboveUpper became %s at absolute %f, relative %f [m]\n",
            toCCP (b), position, getPosition());
   }
   // DEBUG ("<  Simulation::isAboveUpper () = %s\n", toCCP (b));
   return b;
}

bool Simulation::isBelowLower()
{
   // DEBUG (">  Simulation::isBelowLower () \n");
   bool b = (movingForward ? isBelowLowerGoingDown() : isBelowLowerGoingUp());
   if (b != oldBelowLower)
   {
      oldBelowLower = b;
      DEBUG("   Simulation: isBelowLower became %s at absolute %f, relative %f [m]\n",
            toCCP (b), position, getPosition());
   }
   // DEBUG ("<  Simulation: isBelowLower () = %s\n", toCCP (b));
   return b;
}

// liftencoder
float Simulation::getPosition()
{
   float p = position + offset;

   if (!liftEncoder.isEqual (p, previousPosition))
   {
      // DEBUG (">< Sim::getPosition: position = %f, offset = %f, returned %f m\n", position, offset, p);
      previousPosition = p;
   }
   return p;
}

bool Simulation::setOffset(float ofsChg)
{
   offset += (ofsChg);
   DEBUG(">< Sim: setOffset (%f); new offset = %f\n", ofsChg, offset);
   return true;
}

// VFD
bool Simulation::isPowered()
{
   return true;
}

bool Simulation::setSpeed(uint8_t _Hz)
{
   DEBUG(">< Sim set speed to %d [Hz]\n", _Hz);
   speed = _Hz;
   if (speed == 0)
   {
      movingForward = movingReverse = false;
      DEBUG("   Sim set speed to 0\n");
   }
   float sign = movingForward ? 1.0 : movingReverse ? -1.0
                                                    :  0.0;
   meterPerMS = sign * toMeterPerMS(speed);
   return true;
}

bool Simulation::brake()
{
   if (movingForward || movingReverse)
   {
      DEBUG(">< Sim: brake\n");
   }

   setSpeed(0);
   return true;
}

void Simulation::forward(bool f)
{
   if (f != movingForward)
   {
      DEBUG(">< Sim: forward = %s\n", toCCP(f));
      movingForward = f;
   }
   if (f)
   {
      meterPerMS = toMeterPerMS(speed);
      movingReverse = false;
   }
   else
      meterPerMS = 0.0;
}

void Simulation::reverse(bool r)
{
   if (r != movingReverse)
   {
      DEBUG(">< Sim: reverse = %s\n", toCCP(r));
      movingReverse = r;
   }
   if (r)
   {
      meterPerMS = -toMeterPerMS(speed);
      movingForward = false;
   }
   else
   {
      meterPerMS = 0.0;
   }
}

bool Simulation::reset(bool x)
{
   if (x && (movingReverse || movingForward))
   {
      // DEBUG (">< Sim reset\n");
      setSpeed(0);
   }
   return true;
}

#endif
