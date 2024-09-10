//
// liftposition.h -- methods for positioning the lift
//
#ifndef _LIFTPOSITION_H
#define _LIFTPOSITION_H
#include "simulation.h"
#include <Arduino.h>
#include "hsi.h"
#include "Timer.h"


// Note: for all modules that return a bool: true means success, false means fail.

class LiftPosition {
  public:
   bool setup ();
   void loop ();

   bool moveUp ();      // start move up
   bool moveDown ();    // start move down
   bool homeMoveUp   (bool upper, bool initial);
   bool homeMoveDown (bool upper, bool initial);
   bool stop ();       // stop movement
   bool clear ();      // stop movement and reset VFD
   bool stopped ();            // T if stopped
   bool aMoveFinished ();      // T if a regular move finished
   bool aHomeStepFinished (bool &complete);  // T if a step in the homing process finished; complete T if completely finished
 private:

   uint8_t  distanceToSpeed (float meters);   // calc speed from distance
   uint32_t distanceToTime  (float meters);   // estimate max move time from distance
   bool     startMove (bool up, bool fast);
   bool     startMoveTo (float destination);       // abs move; takes care of ranges
   bool     startMoveRelative (float meters);      // rel move; takes care of ranges; negative is up
   bool     checkRange (float meters);
   bool     checkMoving (bool movingUp);
   bool     homeMove (bool up, bool upper, bool initial);

   bool   moveBeyondEndstop (const bool upper, const bool above, const bool fast);
   void   setOffsets        (float offset);
   void   setLowerOffsets   (float offset);

   float  moveDestination;      // position to move to
   bool   moving;               // T if a move is active
   bool   movingUp;             // we are moving up
   bool   lastMoveUp;
   bool   homingUpper;
   bool   homingLower;
   bool   upperHomed;           // T if upper homed
   bool   oldUpperHome;
   bool   lowerHomed;           // T if lower homed
   bool   oldLowerHome;
   int    homePhase;
   int    warningCount;

   bool   moveIntoEndstop;      // T if we are moving into the endstop
   bool   movePastEndstop;      // T if we should move past the endstop
   bool   doingExtraDistance;   // T if we are moving extra distance past endstop

   Timer  checkMoveTimer;
   Timer  moveTimeoutTimer;

   // The following values (floats) are expressed in meters.
   float  minimumPosition;      // if below, the actual position is set to this
   float  maximumPosition;      // if above, the actual position is set to this
   float  lowerEndstopPosition; // last measured value. The upper endstop position is by definition 0
   float  previousPos;



#if SIMULATION==1
   Simulation *simPtr;
#endif
   // Notes: 
   // - Position 0 is the switch point of the upper end-of-stroke switch when the lift goes UP
   // - Positive direction is down
   // - Distance between top position and bottom position of lift is 2.73 m
   // - The actual position is kept in liftencoder.cpp.
   // - When the lift runs in a physical endstop, the lift rope slips over the drive wheel. 
   //    The incremental encoder on the drive wheel will then show an offset. However, we know that
   //    there is a fixed distance between the trip position of the endstop, and the physical end stop. 
   //    Specifically, at the top side, the encoder value cannot be below a fixed value (e.g. -1530 increments) If the value is below that,
   //    the rope has slipped, and we can set the encoder value to the fixed value. The same story holds at the bottom side. 
   //    Position 0 is the upper electrical endstop position, measured GOING UP


};

extern LiftPosition liftPosition;

#endif


