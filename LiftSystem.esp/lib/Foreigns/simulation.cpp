//
// simulation.cpp -- implementation of simulation class
//
// BSla, 28 jun 2024
//
#include "simulation.h"
#define _DEBUG 0
#include "debug.h"
//#include "liftposition.h"

#if SIMULATION==1

// total stroke of lift is 2.73 meter, i.e. from -1.30 to + 1.43
// endstops. These are absolute positions
const float UEPos        = -1.05;
const float LEPos        =  1.58;
const float backlash     =  0.01; 
const float homeDistance =  0.1;

void Simulation::setup () 
{
   DEBUG (">  Simulation::setup\n");
   emoActive = false;
   position = 0.0;   // represents sensor readout
   offset = 0.0;  
   pwm = 0;        // current pwm value
   movingForward = movingReverse =false;
   previousForward = previousReverse = false;
   oldAboveUpper = oldBelowLower = false;
   speedWarningGiven = posWarningGiven = false;

   DEBUG ("<  Simulation::setup done\n");
}

void Simulation::loop ()
{
   if (loopCount++ % 5250 == 0) {  // corresponds with a move time of 10 seconds for the full stroke
      if (movingForward || movingReverse)
      {
         int8_t s = pwm*3;
         if (s == 0) {
            if (!speedWarningGiven) {
               speedWarningGiven = true;
               DEBUG ("**** Sim: loop %ld: moving, but speed == 0\n", loopCount);
            }
         }
         else speedWarningGiven = false;
         if (movingReverse) {
            s = -s;
         }

         if (!emoActive) position += s;  // simulate EMO power off 

         //DEBUG ("Sim: loop %ld: position = %ld = %f m\n", loopCount, 
         //        position, liftEncoder.toMeters (position));
         int32_t incs = liftEncoder.toIncrements (10.0);
         if (position > incs) {
            position = incs;
            if (!posWarningGiven) {
               DEBUG ("**** Sim: position > 10 meter\n");
               posWarningGiven = true;
            }
         }
         else if (position < -incs) {
           position = -incs;
           if (!posWarningGiven) {
              DEBUG ("**** Sim: position < -10 meter\n");
              posWarningGiven = true;
           }
         }
         else if (posWarningGiven) {
            DEBUG ("   Sim: position = %f is back in range\n", liftEncoder.toMeters (position));
            posWarningGiven = false;
         }
      }
   }
}



bool Simulation::isBelowPhysicalLower ()
{
   return (position > liftEncoder.toIncrements(LEPos + homeDistance))? true:false;

}

bool Simulation::isAbovePhysicalUpper ()
{
   return (position < liftEncoder.toIncrements(UEPos - homeDistance))? true:false;
}

#ifdef GEEN_SLIP
void Simulation::simulateSlip ()
{
  if (isAbovePhysicalUpper ()) {
    // we moved beyond physical endstop, so we slipped
    float upperPhysicalPos = UEPos - homeDistance;
    float distance = position - upperPhysicalPos;
    float oldPosition = getPosition ();
    setOffset (distance);
    position = upperPhysicalPos;
    float newPosition = getPosition ();

    DEBUG ("   SimulateSlip: above; offset = %f, position = %f [m]\n", distance, position);
    if (newPosition != oldPosition) DEBUG ("*** getPosition shifts: oldPosition = %f, newPosition = %f\n", oldPosition, newPosition); 
  }
  else if (isBelowPhysicalLower ()) {
    // we moved beyond physical endstop, so we slipped
    float lowerPhysicalPos = LEPos + homeDistance;
    float distance = position - lowerPhysicalPos;
    float oldPosition = getPosition ();
    setOffset (distance);
    position = lowerPhysicalPos;
    float newPosition = getPosition ();

    DEBUG ("   SimulateSlip: below; offset = %f, position = %f [m]\n", distance, position);
    if (newPosition != oldPosition) DEBUG ("*** getPosition shifts: oldPosition = %f, newPosition = %f\n", oldPosition, newPosition); 
  }
}

#endif

bool Simulation::isAboveUpperGoingUp ()
{
   return (liftEncoder.toMeters(position) < UEPos)? true:false;
}

bool Simulation::isAboveUpperGoingDown ()
{
   return (liftEncoder.toMeters(position) < (UEPos+backlash))? true:false;
}

bool Simulation::isBelowLowerGoingUp ()
{
   return (liftEncoder.toMeters (position) > (LEPos-backlash)) ? true:false;
}

bool Simulation::isBelowLowerGoingDown ()
{
   return (liftEncoder.toMeters(position) > (LEPos)) ? true:false;
}

bool Simulation::isAboveUpper ()
{
   //DEBUG (">  Simulation::isAboveUpper () \n");
   bool b =  (movingForward? isAboveUpperGoingDown ():isAboveUpperGoingUp ()); 
   if (b != oldAboveUpper) {
      oldAboveUpper = b;
      DEBUG ("   Simulation: isAboveUpper became %s at absolute %f, relative %f [m]\n",
              b?"true":"false", liftEncoder.toMeters (position), getPosition ());
   }
   //DEBUG ("<  Simulation::isAboveUpper () = %s\n", toCCP (b));
   return b;
}

bool Simulation::isBelowLower ()
{
   //DEBUG (">  Simulation::isBelowLower () \n");
   bool b = (movingForward? isBelowLowerGoingDown ():isBelowLowerGoingUp ()); 
   if (b != oldBelowLower) {
      oldBelowLower = b;
      DEBUG ("   Simulation: isBelowLower became %s at absolute %f, relative %f [m]\n", 
                  b?"true":"false", liftEncoder.toMeters (position), liftEncoder.toMeters (getPosition ()));
   }
   //DEBUG ("<  Simulation: isBelowLower () = %s\n", toCCP (b));
   return b; 
}


//liftencoder
float Simulation::getPosition () 
{
   int32_t p = position + offset;
   if (p != previousPosition) {
      //DEBUG (">< Sim::getPosition: position = %ld, offset = %ld, returned %ld = %f m\n", position, offset, p, liftEncoder.toMeters (p));
      previousPosition = p;
   }
   return liftEncoder.toMeters (p);
}

bool Simulation::setOffset (float ofsChg) 
{ 
   offset += liftEncoder.toIncrements(ofsChg);
   DEBUG (">< Sim: setOffset (%f); new offset = %f\n", ofsChg, liftEncoder.toMeters (offset));
   return true;
}

// VFD
bool Simulation::isPowered () 
{
   return true;
}



bool Simulation::setSpeed (uint8_t _pwm) 
{
   DEBUG (">< Sim set speed to pwm %d; offset = %f m\n", _pwm, liftEncoder.toMeters(offset));
   pwm = _pwm;
   if (pwm == 0) {
      movingForward = movingReverse = false;
      DEBUG ("   Sim set speed to 0\n");
   }
   return true;
}

bool Simulation::brake ()
{
    if (movingForward || movingReverse ) {
       DEBUG (">< Sim: brake\n");
    }
    movingForward = movingReverse = false;
    setSpeed (0);
    return true;
}

void Simulation::forward (bool f)
{
   if (f != movingForward) {
      DEBUG (">< Sim: forward %s\n", f?"true":"false");
      movingForward = f;
   }
}

void Simulation::reverse (bool r)
{
   if (r!= movingReverse) {
         DEBUG (">< Sim: reverse %s\n", r?"true":"false");
      movingReverse = r;
   }
}

bool Simulation::reset (bool x)
{
   if (x && (movingReverse || movingForward)) {
      //DEBUG (">< Sim reset\n");
      movingReverse = movingForward = false;
      setSpeed (0);
   }
   return true;
}

Simulation sim;

#endif
