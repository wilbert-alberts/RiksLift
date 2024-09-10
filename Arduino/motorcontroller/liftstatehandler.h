//
// liftstatehandler.h 
//
// BSla, 18 aug 2024
//
#ifndef _LIFT_STATE_HANDLER_H
#define _LIFT_STATE_HANDLER_H

#include "liftstate.h"

class LiftStateHandler {
 public:
   void setup ();
   void loop ();
   void commandGoUp ();
   void commandGoDown ();
   void commandStop ();
   void emoHit (bool active);
   void screenHit (bool active);
   LiftState getLiftState () {return currentState -> getLiftState ();}
   State *endStopToState ();
   bool isHomed   (bool upper) {return upper? upperHomed: lowerHomed;}
   void setHomed  (bool upper, bool val) {if (upper) upperHomed=val; else lowerHomed=val;}
   void setHoming (bool upper, bool val) {if (upper) homingUpper=val;else homingLower=val;}
   bool isScreenActive () {return anyScreenActive;}
   bool isEmoActive () {return anyEmoActive;}
 private:
   State *currentState;
   bool   anyScreenActive;
   bool   anyEmoActive;
   bool   upperHomed;
   bool   homingUpper;
   bool   lowerHomed;
   bool   homingLower;
   uint32_t wasUpAt;
   uint32_t wasDownAt;
   uint32_t moveUpTime;
   uint32_t moveDownTime;
   Timer  moveTimeoutTimer;
   Timer  publishTimer;

   void   checkEvents ();
   void   stateChanged (State *oldState, State *currentState);
   void   publishArrival (bool upper);
   void   publishMoveTime (uint32_t time);
   void   publishState ();
   void   dealWithMoveTimes (State *currentState, State *oldState, bool publish);
   void   handleEndstops (State *currentState);
   bool   isMovingState (State *state);
   bool   isMovingUpState (State *state);
   bool   isMovingDownState (State *state);
   bool   isHomingState (State *state);
   bool   isStandstillState (State *state);


};
extern LiftStateHandler liftStateHandler;

#endif