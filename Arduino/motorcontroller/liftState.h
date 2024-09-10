//
// liftState.h -- lift state machine
//
// BSla, 14 Aug 2024
#ifndef _LIFTSTATE_H
#define _LIFTSTATE_H

#include "liftglobals.h"
#include "timer.h"

static const uint32_t liftStopTime(200 MILLISECONDS);
static const bool upper (true);
static const bool lower (false);




class Event {
  public:
   Event (const char *Name): name (Name), active (false) {}
   ~Event () {}
   const char* getName () { return name;}
   void activate ();
   void deactivate () {active = false;}
   bool isActive () {return active;}
  private:
   const char* name;
   bool active;
};

class State {
 public:
   State (const char *Name, LiftState _associatedState): name (Name), associatedState (_associatedState) {}
   ~State () {}
   void showHandleEvent (Event *event)  {
      DEBUG (">< State %s: event %s\n", getName (), event->getName ());      
   }
   virtual State *handleEvent (Event *event)  {
      State *newState = this;
      DEBUG ("**** Base class handleEvent: state %s: event %s\n", getName (), event->getName ());   
      return newState;
   }
   virtual void loop () {
         DEBUG ("**** Base class loop called: state %s\n", getName ());
   }  // called once per loop cycle 
   const char* getName () {return name;}
   LiftState getAssociatedState () {return associatedState;}
   LiftState getLiftState () {return associatedState;}
 private:
   const char* name;
   LiftState   associatedState;
};

extern Event goUpReceived;
extern Event goDownReceived;
extern Event stopReceived;
extern Event emoPushed;
extern Event screenTouched;
extern Event destinationReached;
extern Event homePhaseDone;      
extern Event moveTimeout;
extern Event stopped;


class MovingUp : public State {
public:
  MovingUp(const char *name, LiftState ls)
    : State(name, ls) {}
  State *handleEvent(Event *event) override;
  void loop() override {}
private:
};


class MovingDown : public State {
public:
  MovingDown(const char *name, LiftState ls)
    : State(name, ls) {}
  State *handleEvent(Event *event) override;
  void loop() override {}
private:
};

class HomingUp : public State {
public:
  HomingUp(const char *name, LiftState ls)
    : State(name, ls) {}
  State *handleEvent(Event *event) override;
  void loop() override {}
private:
};


class HomingDown : public State {
public:
  HomingDown(const char *name, LiftState ls)
    : State(name, ls) {}
  State *handleEvent(Event *event) override;
  void loop() override {}
private:
};



class IsUp : public State {
public:
  IsUp(const char *name, LiftState ls)
    : State(name, ls) {}
  State *handleEvent(Event *event) override;
  void loop() override {}
private:
};


class IsDown : public State {
public:
  IsDown(const char *name, LiftState ls)
    : State(name, ls) {}
  State *handleEvent(Event *event);
  void loop() override {}
private:
};

class Stopping : public State {
public:
  Stopping(const char *name, LiftState ls)
    : State(name, ls) {}
  State *handleEvent(Event *event) override;
  void loop() override {
    if (stopTimer.hasExpired()) {
      stopped.activate();
      stopTimer.stop();
    }
    else if (!stopTimer.isActive()) {
      stopTimer.start(liftStopTime);
    }
  }
private:
  Timer stopTimer;
};


class Standstill : public State {
public:
  Standstill(const char *name, LiftState ls)
    : State(name, ls) {}
  void activate() {}  // behavior when activated
  State *handleEvent(Event *event) override;
  void loop() override {}
private:
};





extern MovingUp    movingUp;
extern MovingDown  movingDown;
extern HomingUp    homingUp;
extern HomingDown  homingDown;
extern IsUp        isUp;
extern IsDown      isDown;
extern Stopping    stopping;
extern Standstill  standstill;



extern Event *allEvents[];
extern const int nEvents;
#endif