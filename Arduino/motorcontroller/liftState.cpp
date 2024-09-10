//
//  liftState.cpp
//
// BSla, 14 Aug 2024
//
#include <Arduino.h>
#define _DEBUG 1
#include "debug.h"
#include "timer.h"
#include "liftposition.h"
#include "mainapp.h"

#include "liftstatehandler.h"



//***************  Events *******************************************

void Event::activate() {
  DEBUG(">< Event %s activate(d)\n", getName());
  active = true;
}

Event goUpReceived("Go Up received");      
Event goDownReceived("Go Down received");  // from MQTT
Event stopReceived("Stop received");      // from MQTT
Event emoPushed("EMO pushed");           // emo pushed
Event screenTouched("Screen touched");
Event destinationReached("Destination reached");
Event homePhaseDone("home phase done");      
Event stopped("stopped");
Event moveTimeout ("move timeout");

Event *allEvents[] = {
  &goUpReceived,                   //
  &goDownReceived,                 //
  &stopReceived,                   // we received a stop command
  &emoPushed,                      // an emo was pushed
  &screenTouched,                  // a screen was touched
  &destinationReached,             // we reached the destination
  &homePhaseDone,                  // a homing phase (a move up or down) was finished
  &stopped,                        // deceleration phase finished
  &moveTimeout                     // a move did not finish in time
};

const int nEvents = sizeof (allEvents) / sizeof (Event *);

// **************************************** States *******************************************
// type    instance     name          associated liftstate
MovingUp   movingUp    ("MovingUp",   lsMovingUp);
HomingUp   homingUp    ("HomingUp",   lsMovingUp);
IsUp       isUp        ("isUp",       lsUp);
MovingDown movingDown  ("MovingDown", lsMovingDown);
HomingDown homingDown  ("HomingDown", lsMovingDown);
IsDown     isDown      ("isDown",     lsDown);
Stopping   stopping    ("stopping",   lsStopping);
Standstill standstill  ("standstill", lsStandstill);



//************************************ any state handle event

State *MovingUp::handleEvent(Event *event) {
  State *newState = this;
  showHandleEvent(event);

  if ((event == &stopReceived) ||
      (event == &emoPushed)    ||
      (event == &screenTouched)  ) {
    event->deactivate ();
    liftPosition.stop ();
    newState = &stopping;    
  }
  else if (event == &destinationReached) {
    event->deactivate();
    newState = &isUp;
  }
  else if (event == &moveTimeout) {
    event->deactivate ();
    liftPosition.clear ();
    newState = liftStateHandler.endStopToState ();
  }
  else {
    event->deactivate ();
    DEBUG("**** LiftState: unexpected event %s encountered in state %s; ignoring\n", event->getName(), this->getName());
  }
  return newState;
}

State *MovingDown::handleEvent(Event *event) {
  State *newState = this;
  showHandleEvent(event);
  if ((event == &stopReceived) ||
      (event == &emoPushed)    ||
      (event == &screenTouched)  ) {
    event->deactivate();
    liftPosition.stop();
    newState = &stopping;
  }
  else if (event == &destinationReached) {
    event->deactivate();
    newState = &isDown;
  }
  else if (event == &moveTimeout) {
    event->deactivate ();
    liftPosition.clear ();
    newState = liftStateHandler.endStopToState ();
  }
  else {
    event->deactivate ();
    DEBUG("**** LiftState: unexpected event %s encountered in state %s; ignoring\n", event->getName(), this->getName());
  }
  return newState;
}

State *HomingUp::handleEvent(Event *event) {
  State *newState = this;
  showHandleEvent(event);
  bool operatorOK = (!liftStateHandler.isScreenActive () && !liftStateHandler.isEmoActive ());
  if ((event == &stopReceived) ||
      (event == &emoPushed)    ||
      (event == &screenTouched)  ) {
    event->deactivate();
    liftPosition.stop();
    newState = &stopping;
  }
  else if (event == &destinationReached) {
    event->deactivate();
    newState = &isUp;
    DEBUG ("   HomingUp: event DestinationReached; we are done homing\n");
    liftStateHandler.setHomed (upper, true);
    liftStateHandler.setHoming (upper, false);
  } 
  else if (event == &homePhaseDone) {
    event->deactivate();
    if (operatorOK) {
       liftPosition.homeMoveDown (upper, false);
       newState = &homingDown;
    }
    else newState = &isUp;
  } 
  else if (event == &moveTimeout) {
    event->deactivate ();
    liftPosition.clear ();
    newState = liftStateHandler.endStopToState ();
  }
  else {
    event->deactivate ();
    DEBUG("**** LiftState: unexpected event %s encountered in state %s; ignoring\n", event->getName(), this->getName());
  }
  return newState;
}

State *HomingDown::handleEvent(Event *event) {
  State *newState = this;
  showHandleEvent(event);
  bool operatorOK = (!liftStateHandler.isScreenActive () && !liftStateHandler.isEmoActive ());
  if ((event == &stopReceived) ||
      (event == &emoPushed)    ||
      (event == &screenTouched)  ) {
    event->deactivate();
    liftPosition.stop();
    newState = &stopping;
  }
  else if (event == &destinationReached) {
    event->deactivate();
    newState = &isDown;
    DEBUG ("   HomingDown: event DestinationReached; we are done homing\n");
    liftStateHandler.setHomed  (lower, true);
    liftStateHandler.setHoming (lower, false);
  } 
  else if (event == &homePhaseDone) {
    event->deactivate();
    if (operatorOK) {
       liftPosition.homeMoveUp (upper, false);
       newState = &homingUp;
    }
    else newState = &isDown;
  } 
  else if (event == &moveTimeout) {
    event->deactivate ();
    liftPosition.clear ();
    newState = liftStateHandler.endStopToState ();
  }
  else {
    event->deactivate ();
    DEBUG("**** LiftState: unexpected event %s encountered in state %s; ignoring\n", event->getName(), this->getName());
  }
  return newState;
}


State *IsUp::handleEvent(Event *event) {
  State *newState = this;
  showHandleEvent(event);
  bool operatorOK = (!liftStateHandler.isScreenActive () && !liftStateHandler.isEmoActive ());
  if (event == &goDownReceived) {
    event->deactivate();
    if (operatorOK) {
       if (liftStateHandler.isHomed (lower)) {
          liftPosition.moveDown ();
          newState = &movingDown;
       }
       else {
          // first phase of home lower
          liftPosition.homeMoveDown (lower, true);
          newState = &homingDown;
       }
    }
  }
  else if ((event == &goUpReceived)  ||
           (event == &stopReceived)  ||
           (event == &emoPushed)     ||
           (event == &screenTouched)    ) {
     // ignore
     event->deactivate ();
  }
  else {
    event->deactivate ();
    DEBUG("**** LiftState: unexpected event %s encountered in state %s; ignoring\n", event->getName(), this->getName());
    event->deactivate();
  }
  return newState;
}

State *IsDown::handleEvent(Event *event) {
  State *newState = this;
  showHandleEvent(event);
  bool operatorOK = (!liftStateHandler.isScreenActive () && !liftStateHandler.isEmoActive ());
  if (event == &goUpReceived) {
    event->deactivate();
    if (operatorOK) {
       if (liftStateHandler.isHomed (upper)) {
          liftPosition.moveUp ();
          newState = &movingUp;
       }
       else {
          // first phase of home upper
          liftPosition.homeMoveUp (upper, true);
          newState = &homingUp;
       }
    }
  }
  else if ((event == &goDownReceived)  ||
           (event == &stopReceived)  ||
           (event == &emoPushed)     ||
           (event == &screenTouched)    ) {
     // ignore
     event->deactivate ();
  }
  else {
    event->deactivate ();
    DEBUG("**** LiftState: unexpected event %s encountered in state %s; ignoring\n", event->getName(), this->getName());
    event->deactivate();
  }
  return newState;
}

State *Standstill::handleEvent(Event *event) {
  State *newState = this;
  showHandleEvent(event);
  bool operatorOK = (!liftStateHandler.isScreenActive () && !liftStateHandler.isEmoActive ()); 
  if (event == &goUpReceived) {
    event->deactivate();
    if (operatorOK) {
       if (liftStateHandler.isHomed (upper)) {
          liftPosition.moveUp();
          newState = &movingUp;
       }
       else {
          // initial home upper
          liftPosition.homeMoveUp   (upper, true);       
          newState = &homingUp;
       }
    } 
  }
  else if (event == &goDownReceived) {
     event->deactivate();
     if (operatorOK) {
        if (liftStateHandler.isHomed (lower)) {
           liftPosition.moveDown ();
           newState = &movingDown;
        }
        else {
           // initial home lower
           liftPosition.homeMoveDown (lower, true);
           newState = &homingDown;
        }
     }
  }
  else if ((event == &stopReceived) ||
           (event == &stopped)      ||
           (event == &emoPushed)    ||
           (event == &screenTouched)||
           (event == &moveTimeout)    ) {
     // ignore  
     event->deactivate ();
  }
  else {
     event->deactivate ();
     DEBUG("**** LiftState: unexpected event %s encountered in state %s; ignoring\n", event->getName(), this->getName());
     event->deactivate();
  }
  return newState;
}

State *Stopping::handleEvent(Event *event) {
  State *newState = this;
  showHandleEvent(event);
  if ((event == &goUpReceived)   ||
      (event == &goDownReceived) ||
      (event == &stopReceived)   ||
      (event == &emoPushed)      ||
      (event == &screenTouched)  ||
      (event == &moveTimeout)      ) {
     // ignore  
     event->deactivate ();
  }
  else if (event == &stopped) {
    event->deactivate();
    newState = liftStateHandler.endStopToState();
  } 
  else {
    DEBUG("   LiftState: unexpected event %s encountered in state %s; ignoring\n", event->getName(), this->getName());
    event->deactivate();
  }
  return newState;
}