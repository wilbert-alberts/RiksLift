//
// RLsupp.h -- RL supplementary code
//
// BSla, 24 Aug 2024

#ifndef _RLSUPP_H
#define _RLSUPP_H
#include "liftglobals.h"

#include "timer.h"

class RLSupp {
public:
   RLSupp ()  {}
   ~RLSupp () {}

   void setup ();              // called from main    
   void loop ();
   void setState (LiftState ls) {liftState = ls;}    // from callbacks
   void setButtonState (bool Upper, bool pressed);
   void setEMOState    (bool Upper, bool active);
   void setScreenState (bool Upper, bool active);
private:
   bool buttonsPushed [2];    // current state of buttons
   bool oldPushed [2];        // to detect changes
   bool emosActive [2];       // current state of EMOs
   bool screensActive [2];    // current state of screens
   LiftState liftState;       // most recently reported lift state
   Timer commandDelayTimer;   
   bool doCommand;            // T if a move command is to be sent later
   bool goUp;                 // T if the move must go up

   bool anyEmoOrScreenActive ();
   bool anyButtonEvent (bool &Upper);
   void sendGoCommand (bool up);

   void subscribeMQTT ();
};

extern RLSupp rlSupp;

#endif


