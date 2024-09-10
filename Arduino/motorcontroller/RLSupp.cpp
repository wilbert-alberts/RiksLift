//
// RLSupp.cpp -- RL supplementary code
//
// This code supplements the code in RL.exe. It provides for the situation that a move was started, but for some reason the move did not finish.
// In that case the lift is in the state Standstill, RL waits for an endstop to become true but that will not happen.
// This RLSupp.cpp checks for a button press and translates it into a move command.
//
// Note that the interface between this module and the motor controller only consists of MQTT messages.
//
// BSla, 24 Aug 2024
//
#include <Arduino.h>
#include "BrokerLink.h"
#include "liftglobals.h"
#define _DEBUG 1
#include "debug.h"

#include "RLsupp.h"

static const int lower = 0;
static const int upper = 1;

RLSupp rlSupp;

bool RLSupp::anyEmoOrScreenActive ()
{
   bool r = false;

   for (int i = lower; i <= upper; i++)
   {
      r = r || emosActive [i];
      r = r || screensActive [i];
   }
   return r;
}

bool RLSupp::anyButtonEvent (bool &Upper)
{
   bool r = false;
   for (int i = lower; i <= upper; i++) {
      if (buttonsPushed [i]) {
         if (!oldPushed [i]) {
            Upper = (i == upper);
            r = true;
            oldPushed [i] = buttonsPushed [i];
         }
      }
      else oldPushed [i] = buttonsPushed [i]; // released
   }

   if (r) { 
      //DEBUG ("   RLSupp::anyButtonEvent: %s button was pushed\n", Upper?"upper":"lower");
   }
   return r;
}

void RLSupp::sendGoCommand (bool up)
{
   Topic topic (mqtLift); topic += mqsCommand;
   String payload (up?"goUp":"goDown");
   brokerLink.publish (topic, payload, false);
   //DEBUG (">< RLSupp::sendGoCommand::topic %s, payload %s\n", topic.get().c_str (), payload.c_str ());
}

void RLSupp::setup ()
{
   for (int i = lower; i <= upper; i++)
   {
      buttonsPushed [i] = false;
      emosActive [i]    = false;
      screensActive [i] = false;
      doCommand         = false;
      goUp              = false;
   }
   commandDelayTimer.stop ();   
   subscribeMQTT ();
}

bool isStillState (LiftState ls) {
  return (ls == lsUp || ls == lsDown || ls == lsStandstill);
}


void RLSupp::loop ()
{
   bool Upper;
   if (anyButtonEvent (Upper)) {
      DEBUG (">  RLSupp: a button event (u = %s)\n", toCCP(Upper));
      if ((isStillState(liftState)) && (!anyEmoOrScreenActive())) {
         DEBUG ("   RLSupp: still state and no screen active; triggering move\n");
         doCommand = true;
         goUp = Upper;
         commandDelayTimer.start (200 MILLISECONDS);   // give RL time to do its thing
      }
   }
   if (commandDelayTimer.hasExpired ()) {
      //DEBUG ("   RLSupp: timer expired\n");
      commandDelayTimer.stop ();
      if (doCommand && (isStillState (liftState)) && (!anyEmoOrScreenActive ())) {
         if (liftState == lsUp) goUp = false; 
         else if (liftState == lsDown) goUp = true;
         // else lift state = standstill and we use the direction from the button
         DEBUG ("<  RLSupp: throwing in Go command; up = %s\n", toCCP(goUp));
         sendGoCommand (goUp);
      }
      doCommand = false;
   }
}

void RLSupp::setButtonState (bool Upper, bool pressed)
{
   int i = Upper?1:0;
   buttonsPushed [i] = pressed;
   //DEBUG ("><  RLSupp: setButtonState: %s button was %s\n", Upper?"upper":"lower", pressed?"pressed":"released");
}

void RLSupp::setEMOState (bool Upper, bool active)
{
   int i = Upper?1:0;
   emosActive [i] = active;
   //DEBUG ("><  RLSupp: setEMOState: %s EMO was %s\n", Upper?"upper":"lower", active?"pressed":"released");
}

void RLSupp::setScreenState (bool Upper, bool active)
{
   int i = Upper?1:0;
   screensActive [i] = active;
   //DEBUG ("><  RLSupp: setScreenState: %s screen was %s\n", Upper?"upper":"lower", active?"touched":"released");
}


static void LiftStatusCB (const String &payload, const size_t size)
{
   //DEBUG ("><  LiftStatusCB (payload = %s\n", payload.c_str ());
   LiftState ls = stringToLiftState (payload);
   rlSupp.setState (ls);
   //DEBUG ("><  RLSupp: liftStatusCB: liftState = %s\n", liftStateToString (ls));
}

static void upperButtonCB (const String& payload, const size_t size)
{
   rlSupp.setButtonState (true, (payload == mqsPressed));
}

static void lowerButtonCB (const String& payload, const size_t size)
{
   rlSupp.setButtonState (false, (payload == mqsPressed));
}

static void upperEmergencyButtonCB (const String& payload, const size_t size)
{
   rlSupp.setEMOState (true, (payload == mqsTriggered));
}

static void lowerEmergencyButtonCB (const String& payload, const size_t size)
{
   rlSupp.setEMOState (false, (payload == mqsTriggered));
}

static void upperScreenCB (const String& payload, const size_t size)
{
   rlSupp.setScreenState (true, (payload == mqsTriggered));
}

static void lowerScreenCB (const String& payload, const size_t size)
{
   rlSupp.setScreenState (false, (payload == mqsTriggered));
}

void RLSupp::subscribeMQTT ()
{
   DEBUG (">  RLSupp::subscribeMQTT ()\n");
   bool r = true;
   bool d = (_DEBUG == 1);
   Topic topic (mqtLift); topic += mqsStatus;
   r = r && brokerLink.subscribe (topic, LiftStatusCB, d);
   if (!r) DEBUG ("   RLSupp:subscribe: brokerLink subscribe returned false on topic %s\n", topic.get ().c_str ());

   topic.set (mqtButton); topic += mqsUp; topic += mqsStatus;
   r = r && brokerLink.subscribe (topic, upperButtonCB, d);
   if (!r) DEBUG ("   RLSupp:subscribe: brokerLink subscribe returned false on topic %s\n", topic.get ().c_str ());

   topic.set (mqtButton); topic += mqsDown; topic += mqsStatus;
   r = r && brokerLink.subscribe (topic, lowerButtonCB, d);
   if (!r) DEBUG ("   RLSupp:subscribe: brokerLink subscribe returned false on topic %s\n", topic.get ().c_str ());

   topic.set (mqtIntrusion); topic += mqsUp; topic += mqsStatus;
   r = r && brokerLink.subscribe (topic, upperScreenCB, d);
   if (!r) DEBUG ("   RLSupp:subscribe: brokerLink subscribe returned false on topic %s\n", topic.get ().c_str ());

   topic.set (mqtIntrusion); topic += mqsDown; topic += mqsStatus;
   r = r && brokerLink.subscribe (topic, lowerScreenCB, d);
   if (!r) DEBUG ("   RLSupp:subscribe: brokerLink subscribe returned false on topic %s\n", topic.get ().c_str ());

   topic.set (mqtEmergencyButton); topic += mqsUp; topic += mqsStatus;
   r = r && brokerLink.subscribe (topic, upperEmergencyButtonCB, d);
   if (!r) DEBUG ("   RLSupp:subscribe: brokerLink subscribe returned false on topic %s\n", topic.get ().c_str ());

   topic.set (mqtEmergencyButton); topic += mqsDown; topic += mqsStatus;
   r = r && brokerLink.subscribe (topic, lowerEmergencyButtonCB, d);
   if (!r) DEBUG ("   RLSupp:subscribe: brokerLink subscribe returned false on topic %s\n", topic.get ().c_str ());

   if (r) {DEBUG ("<  RLSupp::subscribeMQTT () = OK\n");}
   else   {DEBUG ("*** RLSupp::subscribeMQTT () FAILED\n");}
}