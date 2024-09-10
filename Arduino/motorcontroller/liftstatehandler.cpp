//
// liftstatehandler.cpp
//
// BSla, 18 aug 2024

#include <Arduino.h>

#define _DEBUG 1
#include "debug.h"
#include "timer.h"
#include "liftposition.h"
#include "endstop.h"
#include "brokerLink.h"
#include "mainapp.h"

#include "liftstatehandler.h"

#define PUBLISH_INTERVAL (10 SECONDS)


LiftStateHandler liftStateHandler;

static const uint32_t MoveTimeoutTime (2 MINUTES); 

void LiftStateHandler::setup() {
  DEBUG (">  LiftStateHandler::setup\n");
  liftPosition.setup();
  currentState = endStopToState ();
  mainApp.publishState (currentState -> getAssociatedState ());
  anyScreenActive = false;
  anyEmoActive = false;
  lowerHomed = homingLower = upperHomed = homingUpper = false;
  wasUpAt = wasDownAt = moveUpTime = moveDownTime = 0;
  publishTimer.start (PUBLISH_INTERVAL);
  DEBUG ("<  LiftStateHandler::setup\n");
}

void LiftStateHandler::checkEvents() 
{
  //DEBUG (">  LiftStateHandler::checkEvents () \n");
  // check if movements have finished
  if (homingUpper || homingLower) {
     bool complete;
     if (liftPosition.aHomeStepFinished (complete)) {
        DEBUG ("   LiftStateHandler::checkEvents: a home step finished (complete = %s)\n", toCCP (complete));
        if (complete) destinationReached.activate ();
        else homePhaseDone.activate();
        moveTimeoutTimer.stop ();
     }
  }
  else if (liftPosition.aMoveFinished()) {
    DEBUG ("   LiftStateHandler::checkEvents: not homing; a move finished\n");
    destinationReached.activate();
    moveTimeoutTimer.stop ();
  }

  else if (moveTimeoutTimer.hasExpired ()) {
     moveTimeout.activate ();
     moveTimeoutTimer.stop ();
     DEBUG ("  LiftStateHandler: we have a move timeout\n");
  }
  //DEBUG ("<  LiftStateHandler::checkEvents\n");
}

void LiftStateHandler::loop() 
{
  liftPosition.loop();
  currentState->loop();
  checkEvents();                          // check for conditions and activate matching events

  for (int i = 0; i < nEvents; i++) {
     Event *thisEvent = allEvents[i];
     // DEBUG ("   LSH::loop: checking if %s is active: isActive = %s\n", thisEvent->getName (), toCCP (thisEvent -> isActive()));
     if (thisEvent->isActive()) {                  // we found an active event
        State *oldState = currentState;
        //DEBUG ("  LSH Loop: calling %s.handleEvent (event %s) \n", currentState->getName (), thisEvent->getName ());
        currentState = currentState->handleEvent(thisEvent);  // make the current state deal with the event

        if (currentState != oldState) {    // we changed state
           stateChanged (oldState, currentState);
        }
     }
  }
  if (publishTimer.hasExpired ()) {
      publishTimer.restart ();
      publishState ();
  }
  // DEBUG ("<  LiftStateHandler::loop\n");
}


// we changed state
void LiftStateHandler::stateChanged (State *oldState, State *currentState)
{
   if (isMovingState (currentState)) {
      moveTimeoutTimer.start (MoveTimeoutTime);  // we started a moving state; start the timer
   }

   // do not publish the transient states etc while homing
   bool publish = !(isHomingState (currentState) && isHomingState (oldState));

   // otherwise do publish the new state
   if (publish) mainApp.publishState (currentState -> getAssociatedState ());
   dealWithMoveTimes (currentState, oldState, publish);
   handleEndstops (currentState);
   publishTimer.restart ();
}




bool LiftStateHandler::isMovingState (State *state)
{
   return (state == &movingUp   ||
           state == &movingDown ||
           state == &homingUp   ||
           state == &homingDown   );
}

bool LiftStateHandler::isMovingUpState (State *state)
{
   return (state == &movingUp   ||
           state == &homingUp   );
}

bool LiftStateHandler::isMovingDownState (State *state)
{
   return (state == &movingDown ||
           state == &homingDown   );
}

bool LiftStateHandler::isHomingState (State *state)
{
   return (state == &homingUp   ||
           state == &homingDown   );
}

bool LiftStateHandler::isStandstillState (State *state)
{
   return (state == &isUp       ||
           state == &standstill ||
           state == &isDown         );
}

void LiftStateHandler::publishMoveTime (uint32_t moveTime)
{
   DEBUG (">< LiftStateHandler::publishMoveTime (movetime = %d ms)\n", moveTime);
   Topic topic (mqtLift); topic += mqsXpMoveTime;
   int seconds = (moveTime+500)/1000;
   String payload (seconds);
   brokerLink.publish (topic, payload, false);  
}

void LiftStateHandler::publishArrival (bool upper)
{
   DEBUG (">< LiftStateHandler::publishArrival (upper = %s)\n", toCCP(upper));
   Topic topic (mqtLiftSensor); topic += upper? mqsUp: mqsDown; topic += mqsStatus;
   String payload ("arrivalDetected");
   brokerLink.publish (topic, payload, false);  
}

void LiftStateHandler::dealWithMoveTimes (State *currentState, State *oldState, bool publish)
{
   if (currentState == &standstill) {
      wasUpAt = wasDownAt = 0;           // the measured move times are from up to down or vv, so no longer valid
   }

   // handle move starts
   if (isMovingDownState (currentState)) {                // we just started a move down
      DEBUG ("   LiftStateHandler::dealWithMoveTimes: moving down\n" );
      if (oldState == &isUp) wasUpAt = millis ();         // remember when we were up last
      if (moveDownTime == 0) moveDownTime = 25 SECONDS;   // we have no valid measurement; preset move time to 25 seconds
      DEBUG ("   LiftStateHandler: wasUpAt = %d, moveDownTime = %d [s]\n", wasUpAt/1000, moveDownTime/1000);
      if (publish) publishMoveTime (moveDownTime);
   }

   if (isMovingUpState (currentState))          {         // we just started a move up
      DEBUG("   LiftStateHandler::dealWithMoveTimes: moving up\n" );
      if (oldState == &isDown) wasDownAt = millis ();     // remember when we were down last
      if (moveUpTime == 0) moveUpTime = 25 SECONDS;       // we have no valid measurement; preset move time to 25 seconds
      DEBUG ("   LiftStateHandler: wasDownAt = %d, moveUpTime = %d [ms]\n", wasDownAt, moveUpTime);
      if (publish) publishMoveTime (moveUpTime);
   }

   // handle move completions
   if ((currentState == &isUp) && (oldState == &movingUp)) {
      if (wasDownAt != 0)  moveUpTime = millis () - wasDownAt;  // we finished a move up; calculate the time it took
   }

   if ((currentState == &isDown) && (oldState == &movingDown)) {
      if (wasUpAt != 0) moveDownTime = millis () - wasUpAt;   // we finished a move down; calculate the time it took
   }
}

void LiftStateHandler::handleEndstops (State *currentState)
{
   if (currentState == &isUp) {
      publishArrival (true);
      if (!upperEndstop.reached ()) {
          DEBUG ("*** LiftStateHandler:handleEndstops: new state = isUp, but upper endstop not reached\n");
      }
   }

   if (currentState == &isDown) {
      publishArrival (false);
      if (!lowerEndstop.reached ()) {
         DEBUG ("*** LiftStateHandler::handleEndstops: new state = isDown, but lower endstop not reached\n");
      }
   }
}

State *LiftStateHandler::endStopToState() {
  State *newState = &standstill;
  if (upperEndstop.reached ()) newState = &isUp;
  else if (lowerEndstop.reached ()) newState = &isDown;
  DEBUG(">< LiftStateHandler::endstopToState: state = %s\n", newState->getName());
  return newState;
}

void LiftStateHandler::commandGoUp() {
  DEBUG(">< LiftStateHandler::commandGoUp ()\n");
  if (!upperHomed) homingUpper = true;
  goUpReceived.activate();
}

void LiftStateHandler::commandGoDown() {
  DEBUG(">< LiftStateHandler::commandGoDown ()\n");
  if (!lowerHomed) homingLower = true;
  goDownReceived.activate();
}

void LiftStateHandler::commandStop() {
  DEBUG(">< LiftStateHandler::commandStop ()\n");
  stopReceived.activate();
}

void LiftStateHandler::emoHit(bool active) {
  DEBUG(">< LiftStateHandler::emoPressed ()\n");
  if (active) emoPushed.activate();
  anyEmoActive = active;
}

void LiftStateHandler::screenHit(bool active) {
  DEBUG(">< LiftStateHandler::screenHit ()\n");
  screenTouched.activate();
  anyScreenActive = active;
}

void LiftStateHandler::publishState ()
{
    LiftState ls = currentState -> getAssociatedState ();
    DEBUG ("   LSH: publishing state %s\n",liftStateToString (ls));
    mainApp.publishState (ls);
}