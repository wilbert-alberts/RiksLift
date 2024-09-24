//
// identity.h -- check floor controller identity
//
// BSla, 7 Jul 2024

#ifndef _IDENTITY_H
#define _IDENTITY_H
#include <Arduino.h>
#include "liftglobals.h"

#define GPIO_VAL_UPPER_FLOOR 1  // value of jumper setting for upper floor; other is 0

class Identity {
  public:
   Identity ()  {if (!isSetup) setup ();}
   ~Identity () {}

   bool   isFloorController ();
   bool   isUpperFloor ();
   String getOwnID (); 
   String whichFloor ();
  private:
   void setup ();
   static bool isSetup;
};

extern Identity identity;

#endif
