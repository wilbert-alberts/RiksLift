//
// liftglobals.cpp -- support routines
//
// BSla, 29 jul 2024

#define _DEBUG 0
#include "Debug.h"

#include "liftglobals.h"
#include "identity.h"

// MQTT topics
const char *mqtLift            ("lift");
const char *mqtEmergencyButton ("emergencyButton");
const char *mqtButton          ("button");
const char *mqtIntrusion       ("intrusiondetection");
const char *mqtLiftSensor      ("liftsensor");
const char *mqtDisplay         ("display");
const char *mqtFirmware        ("firmware");

// MQTT subtopics
const char *mqsUp              ("up");
const char *mqsDown            ("down");
const char *mqsMotor           ("motor");

const char *mqsCommand         ("command");
const char *mqsStatus          ("status");
const char *mqsVersion         ("version");
const char *mqsXpMoveTime      ("expectedMoveTime");

// MQTT_commands and statuses

const char *mqcGoUp            ("goUp");
const char *mqcGoDown          ("goDown");
const char *mqcStop            ("stop");

const char *mqsTriggered       ("triggered");
const char *mqsCleared         ("cleared");
const char *mqsPressed         ("pressed");
const char *mqsReleased        ("released");

const char *mqsGoingUp         ("goingUp");
const char *mqsGoingDown       ("goingDown");
const char *mqsIsUp            ("isUp");
const char *mqsIsDown          ("isDown");
const char *mqsStopping        ("stopping");
const char *mqsStandstill      ("standStill");

const char *mqsArrivalDetected ("arrivalDetected");

const char *IDLowerFloor       ("LowerFloor");
const char *IDUpperFloor       ("UpperFloor");
const char *IDMotorControl     ("MotorControl");
const char *IDIllegal          ("*** Illegal ID");


const char *liftCommandToString (LiftCommand lc)
{
   const char *s = "*** Illegal command"; 
   switch (lc) {
      case lcGoUp:   s = mqcGoUp;      break;
      case lcGoDown: s = mqcGoDown;    break;
      case lcStop:   s = mqcStop;      break;
      default:       s = "*** Illegal lift command value"; break;
   }
   return s;
}

LiftCommand stringToLiftCommand (const String &s)
{
  //DEBUG (">< stringToLiftCommand (string = %s)\n", s.c_str ());
  LiftCommand lc (lcNone); 
  if      (s == mqcGoUp)      lc = lcGoUp;
  else if (s == mqcGoDown)    lc = lcGoDown;
  else if (s == mqcStop)      lc = lcStop;

  if (lc == lcNone) DEBUG ("*** Illegal lift command (from string '%s'\n", s.c_str ());
  return lc;
}

const char *liftStateToString (LiftState ls)
{
   const char *s = "*** Illegal state"; 
   switch (ls) {
      case lsMovingUp:   s = mqsGoingUp;      break;
      case lsMovingDown: s = mqsGoingDown;    break;
      case lsUp:         s = mqsIsUp;         break;
      case lsDown:       s = mqsIsDown;       break;
      case lsStopping:   s = mqsStopping;     break;
      case lsStandstill: s = mqsStandstill;   break;
      default:           s = "None";          break;
   }
   return s;
}

LiftState stringToLiftState (const String &s)
{
  DEBUG (">< stringToLiftState (string = %s)\n", s.c_str ());
  LiftState ls (lsNone); 
  if      (s == mqsGoingUp)      ls = lsMovingUp;
  else if (s == mqsGoingDown)    ls = lsMovingDown;
  else if (s == mqsIsUp)         ls = lsUp;
  else if (s == mqsIsDown)       ls = lsDown;
  else if (s == mqsStandstill)   ls = lsStandstill;
  if (ls == lsNone) DEBUG ("*** Illegal lift state (from string '%s'\n", s.c_str ());
  return ls;
}



