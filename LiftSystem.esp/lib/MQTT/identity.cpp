//
// identity.cpp -- check floor controller identity
//
// BSla, 7 Jul 2024
#include "identity.h"
#include <Arduino.h>
#include "hsi.h"

#define _DEBUG 0
#include "debug.h"

Identity identity;

bool Identity::isSetup = false;


void Identity::setup ()
{
   pinMode (GPIO_ID_FLOOR_CONTROLLER, INPUT);
   pinMode (GPIO_ID_WHICH_FLOOR, INPUT);
   isSetup = true;
}

bool Identity::isFloorController ()
{
   if (!isSetup) setup ();
   int8_t pinValue = digitalRead (GPIO_ID_FLOOR_CONTROLLER);
   
   bool ifc = ( pinValue == 1);
   DEBUG (">< Identity::this is %sa floor controller\n", ifc?"":"NOT ");
   return ifc;
}

bool Identity::isUpperFloor ()
{
   if (!isSetup) setup ();
   bool iuf = (digitalRead (GPIO_ID_WHICH_FLOOR) == GPIO_VAL_UPPER_FLOOR);
   DEBUG (">< Identity::isUpperFloor = %s\n", iuf?"true":"false");
   return (iuf);
}

String Identity::getOwnID () 
{
  //DEBUG(">  getOwnID()\n");
  String result = IDIllegal;
  if (!identity.isFloorController()) {
    result = IDMotorControl;
  } else if (identity.isUpperFloor()) {
    result = IDUpperFloor;
  } else {
    result = IDLowerFloor;
  }
  //DEBUG("<  getOwnID() = %s\n", result.c_str());
  return result;
}

String Identity::whichFloor ()
{
   String floor = String ("*** Illegal floor");
   if (!isFloorController ()) {
      DEBUG ("*** Identity::whichFloor: is not a floor controller\n");
   }
   else {
      floor = isUpperFloor ()? mqsUp:mqsDown;
   }
   // DEBUG (">< Identity::whichFloor () = '%s'\n", floor.c_str ());
   return floor;
}

