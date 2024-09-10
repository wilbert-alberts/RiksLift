//
// liftposition.cpp -- class liftposition implementation for Riks Lift
//
// BSla, 15 Jun 2024
//

#include "liftencoder.h"
#include "VevorVFD.h"
#include "hsi.h"
#include "endstop.h"
#include "timer.h"

#define CHECK_MOVE_TIME (1 SECOND)

#include "liftposition.h"

#define _DEBUG 1
#include "debug.h"

const float PastHome          = 0.03;    // movement past endstop during homing 
const float FastSpeedDistance = 0.3;     // above this distance you can move fast
const float LowSpeedDistance  = 0.1;     // above this distance you can move medium
const float PastUpperEndstop  = 0.08;    // range above upper endstop
const float PastLowerEndstop  = 0.08;

LiftPosition liftPosition;

// used throughout this source
static const bool upper = true;
static const bool lower = false;
static const bool up    = true;
static const bool down  = false;
static const bool above = true;
static const bool below = false;
static const bool fast = true;
static const bool slow = false;

#ifdef NIET_GEBRUIKT
static const char *fsToStr(bool fast) {
  return (fast ? "fast" : "slow");
}
static const char *abToStr(bool above) {
  return (above ? "above" : "below");
}
#endif

#if _DEBUG==1
static const char *sfToStr(bool success) {
  return (success ? "success" : "*** FAIL");
}
static const char *ulToStr(bool upper) {
  return (upper ? "upper" : "lower");
}

static const char *udToStr (bool b) {
  return (b ? "up" : "down");
}
static const char *ioToStr (bool b) {
  return (b ? "into" : "out of");
}
#endif






bool LiftPosition::setup() {
  DEBUG(">  LiftPosition::setup\n");
  // clear all class variables
   moveDestination = 0.0;   // position to move to
   moving = false;          // T if a move is active
   movingUp = false;        // we are moving up
   upperHomed = false;      // T if upper homed
   lowerHomed = false;      // T if lower homed
   homePhase = 0;

   moveIntoEndstop    = false; // T if we are moving into the endstop
   movePastEndstop    = false; // T if we should move past the endstop
   doingExtraDistance = false; // T if we are moving extra distance past endstop

   // The following values are expressed in meters.
   minimumPosition      = 0.0;      // if below, the actual position is set to this
   maximumPosition      = 0.0;      // if above, the actual position is set to this
   lowerEndstopPosition = 0.0; // last measured value. The upper endstop position is by definition 0

  // setup all lower modules
#if SIMULATION == 1
  simPtr = sim.getSimPtr();
  simPtr -> setup ();
#endif
  liftEncoder.setup();
  vevorVFD.setup();
  upperEndstop.setup(GPIO_UPPER_ENDSTOP);
  lowerEndstop.setup(GPIO_LOWER_ENDSTOP);

  checkMoveTimer.start (CHECK_MOVE_TIME);

  DEBUG("<  LiftPosition::setup done\n");
  return true;
}

void LiftPosition::loop() 
{
#if SIMULATION == 1
  simPtr->loop();
#endif
  lowerEndstop.loop();
  upperEndstop.loop();
}

bool LiftPosition::aMoveFinished()
// return T if a regular move finished
{
  //DEBUG (">  Liftposition::aMoveFinished? ()\n");
  bool r = false;
  
  if (moving) {
     // decrease speed based on distance to go
     float distanceToGo = moveDestination - liftEncoder.getPosition();
     if (movingUp == (distanceToGo >= 0.0)) {  // moving up means lower position
        // we are done!
        DEBUG ("   Liftpos::aMoveFinished: past destination\n");
        r = true;
        stop();
     } else {
        float speed = distanceToSpeed(distanceToGo);
        vevorVFD.setSpeed(speed, true);
        checkMoving (movingUp);
     }

     // did we have a move timeout?
     if (moveTimeoutTimer.hasExpired ()) {
        DEBUG ("*** LiftPosition: move timeout\n");
        stop ();
     }
  }

  if (r) {
     DEBUG ("<  Liftposition::aMoveFinished = %s, position = %f\n", 
            toCCP (r), liftEncoder.getPosition ());
  }
  return r;
}



bool LiftPosition::checkMoving (bool movingUp)
{
   bool r = true;
   float here = liftEncoder.getPosition ();
   if (lastMoveUp != movingUp) {                   // we are changing direction; reset timer etc
      previousPos = here;
      checkMoveTimer.start ();
      lastMoveUp = movingUp;
      warningCount = 0;
   }
   else {
      float diff = here - previousPos;
      bool isLower = (diff > 1e-5);
      bool isHigher = (diff < -1e-5);
  
      bool isNOK = ((isLower && movingUp) ||
                   (isHigher && !movingUp) );

      if (isNOK && (warningCount++ < 10)) {
         DEBUG ("**** LiftPosition: checkMoving: expect move %s but we are moving %s (here = %f, prev = %f) \n", 
         udToStr (movingUp), udToStr (!isLower), here, previousPos);
         r = false;
      }
      previousPos = here;
      if (!isLower) diff = -diff;
      if (!moving ||(diff > 1e-6)) { // we are not supposed to move or we are indeed moving
         checkMoveTimer.start();
         previousPos = here;
         warningCount = 0;
      }
      else { // should move but no movement;
         if (checkMoveTimer.hasExpired ()) {
            if (warningCount++ < 10) {
              DEBUG ("**** LiftPosition: checkMoving: pos = %f, pos diff = %f, is (almost) zero\n", here, diff);
            }
            vevorVFD.reset ();
            checkMoveTimer.start ();
            r = false;
         }
      }
   }
   return r;
}


bool LiftPosition::aHomeStepFinished(bool &complete)
// return T if the next step in a homing process finished
{
  //DEBUG (">  LiftPosition::aHomeStepFinished ()\n");
  complete = false;
  bool r = false;
  if (moving) {
    if (doingExtraDistance) {
      //DEBUG ("  aHomeStepFinished: waiting for move to finish, pos = %f\n", liftEncoder.getPosition ());
      if (aMoveFinished()) {
         // a relative move into or out of a home sensor finished
         DEBUG("   aHomeStepFinished: extra move into / out of endstop finished\n");
         r = true;
         homePhase++;
      }
    }

    else if (homingUpper || homingLower) {
      bool uer = upperEndstop.reached ();
      bool ler = lowerEndstop.reached ();

      bool reached = homingUpper?uer:ler;
      bool movingIntoEndstop = (movingUp == homingUpper); // if false, moving out of sensor

      if ((oldUpperHome != uer) || (oldLowerHome != ler)) {
         oldUpperHome = uer; oldLowerHome = ler;
         DEBUG ("   aHomeStepFinished: a home sensor switched state; UER = %s, LER = %s\n",
                toCCP (uer), toCCP (ler));
         DEBUG ("   movingIntoEndstop = %s, reached = %s, movePastEndstop = %s\n",
                 toCCP (movingIntoEndstop), toCCP (reached), toCCP (movePastEndstop));
      }

      if (movingIntoEndstop == reached) { // either moved into or out of sensor, 
         if (!movePastEndstop) {  // we are done
            DEBUG("   aHomeStepFinished: moved %s %s endstop\n", 
                 ioToStr (movingIntoEndstop), ulToStr(homingUpper));
            r = true;
            stop();
            homePhase++;
         } 
         else if (!doingExtraDistance) {  // start move past endstop
            DEBUG("   aHomeStepFinished: move further %s %s endstop\n",
                  ioToStr (movingIntoEndstop), ulToStr(movingUp));
            float moveDist = movingUp? -PastHome: PastHome;
            startMoveRelative(moveDist);
            doingExtraDistance = true;
         }
      }
      else if (!checkMoving (movingUp)) {
          
      } // are we still moving the right direction?
    }
    else {
       DEBUG ("**** aHomeStepFinished: doing homing logic but we are not homing\n");
    }

    if (r) {  // we finished a home phase
      if (homePhase >= 4) {
        complete = true;
        if (homingUpper || homingLower) {
          // we are done homing 
          DEBUG("    aHomeStepFinished: homing %s done; setting offsets\n", ulToStr (homingUpper));
          float pos = 0.0;
          if (homingUpper) {
             upperHomed = true; homingUpper = false;
             bool gotOffset = upperEndstop.getEndstopPosition (pos);
             DEBUG ("   aHomeStepFinished: offset is %svalid, offset = %f\n", gotOffset?"":"NOT ", -pos);
             if (gotOffset) setOffsets (-pos);
          }
          else if (homingLower) {
             lowerHomed = true; homingLower = false;
             bool gotPosition = lowerEndstop.getEndstopPosition (pos); 
             DEBUG("    aHomeStepFinished: upper pos is %svalid, pos = %f\n", gotPosition?"":"NOT ", -pos);
             if (gotPosition) {
                lowerEndstopPosition = pos;
                maximumPosition = pos + PastLowerEndstop;
             }
          }
        }
      }
    }
  }
  if (r) {
     DEBUG ("<  LiftPosition::aHomeStepFinished =  %s, homePhase = %d, position = %f\n", 
             toCCP (r), homePhase, liftEncoder.getPosition ());
  }

  return r;
}


bool LiftPosition::moveUp()
// move to top floor. Return T if success
{
  bool r = true;
  DEBUG(">  LiftPosition::Move Up\n");
  if (!upperHomed) {
     DEBUG ("*** LiftPosition: moveUp but upper not homed\n");
     r = false;
  }
  if (r) {
     moving = movingUp = true;
     // move absolute to end pos
     startMoveTo(-PastHome);  // a few cm above the upper home
  } 
  DEBUG("<  LiftPosition::start move Up to %f: %s\n", -PastHome, sfToStr (r));
  return r;
}

bool LiftPosition::moveDown()
// move to lower floor. T if success
{
  DEBUG(">  LiftPosition::Move Down\n");
  bool r = true;
  float downPosition = lowerEndstopPosition + PastHome;
  if (!lowerHomed) {
     DEBUG ("*** LiftPosition: moveDown but lower not homed\n");
     r = false;
  }
  if (r) {
     moving = true; movingUp = false;
     // move absolute to end pos
     startMoveTo(downPosition);
  } 
  DEBUG("<  LiftPosition::start move down to %f: %s\n", downPosition, sfToStr (r));
  return r;
}

bool LiftPosition::homeMove (bool up, bool upper, bool initial)
{
  DEBUG(">  LiftPosition::homeMove (up = %s, upper = %s, initial = %s) (homePhase = %d)\n", 
          toCCP (up), toCCP(upper), toCCP (initial), homePhase);
  movingUp = up;
  moving = true;
  bool r = true;
  if (initial) {
    if (up != upper) {
       DEBUG ("*** homeMove: initial == true, but upper != up (upper = %s, up = %s)\n",
              toCCP (upper), toCCP (up));
       r = false;
    }
    homePhase = 0;
    homingUpper = up;
    homingLower = !up;
    if (up) upperHomed = false; else lowerHomed = false;
  }
  if (homePhase == 0) {  // initial state
    homePhase = 1;
    bool reached = up?upperEndstop.reached () : lowerEndstop.reached ();
    if (!reached) {
      // we are outside of the endstop; move into it
      moveIntoEndstop = true;
      movePastEndstop = false;  // we just need to be in the endstop
      doingExtraDistance = false;
      startMove(up, fast);
    } 
    else {
      // we are already into endstop
      moveIntoEndstop = false;
      oldUpperHome = oldLowerHome = false;
      movePastEndstop = false;  // no need for extra move
      doingExtraDistance = false;
      startMove (up, slow);  // to trigger the completion
    }
  }
  else if (homePhase == 2) {
     // we are moving out of the other endstop
     moveIntoEndstop = false;
     movePastEndstop = true;   // need to move a bit further out
     doingExtraDistance = false;
     startMove (up, slow);
  }   
  else if (homePhase == 3) {
    // second move into endstop
    moveIntoEndstop = true;
    movePastEndstop = true;  // need to move a bit further in
    doingExtraDistance = false;
    if (up) upperEndstop.clearEndstopPosition ();
    else lowerEndstop.clearEndstopPosition ();
    startMove (up, slow);
  }
  DEBUG("<  LiftPosition::home Move %s: %s, homePhase = %d\n", udToStr (up), sfToStr (r), homePhase);
  return r;  
}

bool LiftPosition::homeMoveUp (bool upper, bool initial)
{
   return homeMove (true, upper, initial);
}

bool LiftPosition::homeMoveDown (bool upper, bool initial)
{
   return homeMove (false, upper, initial);
}

bool LiftPosition::stop()
// stop movement
{
  DEBUG(">< Liftposition::stop ()\n");
  bool r = vevorVFD.brake();
  movingUp = moving = false;
  moveTimeoutTimer.stop ();
  //DEBUG("<  Liftposition: stop: %s\n", sfToStr(r));
  return r;
}

bool LiftPosition::clear()
// clear the VFD after an incident (e.g. a timeout)
{
  DEBUG(">  Liftposition::clear ()\n");
  bool r = vevorVFD.reset ();
  if (r) stop ();
  DEBUG("<  Liftposition: clear: %s\n", sfToStr(r));
  return r;
}


bool LiftPosition::checkRange(float position) {
  bool r = true;
  if (upperHomed) {
    if (position < minimumPosition) {
      DEBUG("**** Liftposition::checkRange: position %f m is below minimum %f m\n",
            position, minimumPosition);
      r = false;
    }
  }
  if (lowerHomed) {
    if (position > maximumPosition) {
      DEBUG("**** LiftPosition::checkRange: position %f m is above maximum %f m\n",
            position, maximumPosition);
      r = false;
    }
  }
  return r;
}

bool LiftPosition::startMove (bool up, bool fast)
{
   DEBUG (">  LiftPosition::startMove (up=%s, fast = %s)\n", toCCP (up), toCCP (fast));
   uint8_t speed = fast?VFD_HIGH:VFD_LOW;
   movingUp = up; moving = true;
   bool r = vevorVFD.startMove(movingUp, speed);
   checkMoveTimer.start ();
   moveDestination = movingUp?-10.0:10.0; 
   DEBUG ("<  LiftPosition::startMove: %s\n", sfToStr (r));
   return r;
}

bool LiftPosition::startMoveTo(float destination)
// move to absolute position with range check if calibrated
{
  DEBUG(">  LiftPosition::startMoveTo pos %f m\n", destination);
  bool r = checkRange(destination);
  if (r) moveDestination = destination;
  float here = liftEncoder.getPosition();
  float distanceToMove = destination - here;
  DEBUG("  LiftPosition::startMoveTo: dest = %f, currentPos = %f, dist = %f\n",
        destination, here, distanceToMove);
  r = r && startMoveRelative(distanceToMove);
  DEBUG("<  Liftposition: moveTo: %s\n", sfToStr(r));
  return r;
}

bool LiftPosition::startMoveRelative(float distance)
// relative move, no range check
{
  bool r = true;
  DEBUG(">  Liftposition::startMoveRelative (distance = %f meter)\n", distance);

  moveDestination = liftEncoder.getPosition() + distance;
  uint8_t speed = distanceToSpeed(distance);
  uint32_t moveTime = distanceToTime(distance);
  moveTimeoutTimer.start (moveTime);
  movingUp = (distance < 0);
  moving = true;
  vevorVFD.startMove(movingUp, speed);
  DEBUG("<  Liftposition::startMoveRelative: %s\n", sfToStr(r));
  return r;
}


uint8_t LiftPosition::distanceToSpeed(float meters) {
  uint8_t Hz = 0;
  if (meters < 0.0) meters = -meters;

  if (meters > FastSpeedDistance) Hz = VFD_HIGH;
  else if (meters < LowSpeedDistance) Hz = VFD_LOW;
  else Hz = VFD_MEDIUM;
  //DEBUG (">< Liftpos distance to speed (d = %f meters) speed = %d Hz\n", meters, Hz);
  return Hz;
}

uint32_t LiftPosition::distanceToTime(float meters) {
  uint32_t time = 0;
  if (meters < 0) meters = -meters;

  if (meters < LowSpeedDistance) time = uint32_t(meters * LOW_SPEED_TIME + 1000);  // for 0.0 meters time = 1 second
  else if (meters > FastSpeedDistance) time = uint32_t(meters * HIGH_SPEED_TIME);
  else time = uint32_t(meters * MEDIUM_SPEED_TIME);
  // DEBUG (">< Liftpos distance to time (d = %f meter) time = %d ms\n", meters, time);
  return time;
}


void LiftPosition::setOffsets (float offset)
// Offset the upper and lower positions and the encoder.
// Offset is ADDED to existing values.
//
{
  DEBUG(">< LiftPosition: setOffsets (%f [m])\n", offset);
  upperEndstop.setOffset (offset);
  minimumPosition = -PastUpperEndstop;

  setLowerOffsets (offset);
  liftEncoder.setOffset (offset);
}


void LiftPosition::setLowerOffsets (float offset)
// Offset only the lower endstop position and the maximum position.
// Offset is ADDED to existing values
{
  DEBUG(">< LiftPosition: setLowerOffsets (%f [m])\n", offset);
  lowerEndstop.setOffset (offset);
  lowerEndstopPosition += offset;
  maximumPosition = lowerEndstopPosition + PastLowerEndstop;
}

