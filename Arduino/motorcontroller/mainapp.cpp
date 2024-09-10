//
// mainapp.cpp -- motor controller main application for Riks Lift
//
// BSla, 10 Aug 2024
//

#define _DEBUG 1
#include "debug.h"
#include "timer.h"

#include "identity.h"
#include "brokerLink.h"
#include "liftStateHandler.h"
#include "simulation.h"

#include "mainapp.h"

const char* compileDT = "Motor controller V0.2 " __DATE__ " " __TIME__;

const bool cbVerbose (false);     // call debugs for every callback

const uint32_t PublishStateInterval (30 SECONDS);  // publish the state once every <this interval> 

MainApp mainApp;

void MainApp::setup ()
{
   DEBUG (">  MainApp: setup\n");
   emoTriggered    [0] = emoTriggered    [1] = reportedEmo    =
   screenTriggered [0] = screenTriggered [1] = reportedScreen = false;
   publishedState = lsNone;

   brokerLink.setup ();
   subscribeMQTT ();
   publishFirmwareVersion ();
   liftStateHandler.setup ();
   publishedState = lsStandstill;
   publishStateTimer.start (PublishStateInterval);
   DEBUG ("<  MainApp: setup done\n");
}

void MainApp::loop () 
{
   brokerLink.loop ();
   liftStateHandler.loop ();
   if (publishStateTimer.hasExpired ()) publishState ();
}

void MainApp::publishState  ()
{
   if (cbVerbose) { DEBUG (">  Mainapp::publishState ()\n"); }
   publishStateTimer.restart ();
   String topic ("lift/status");
   String payload (liftStateToString (publishedState));
   brokerLink.publish (topic, payload, false);
   if (cbVerbose) { DEBUG ("  Mainapp::publishState: topic = %s, payload = %s\n", topic.c_str (), payload.c_str ()); }
}

void MainApp::screenHandler (bool upper, bool triggered)
{
   if (cbVerbose) {DEBUG (">  %s screenHandler (triggered = %s)\n", upper?"upper":"lower", toCCP (triggered));}
   int index = (upper?1:0);
   screenTriggered [index] = triggered;
   bool anyTriggered = screenTriggered [0] || screenTriggered [1];
   if (anyTriggered != reportedScreen) {
      reportedScreen = anyTriggered;
      liftStateHandler.screenHit (anyTriggered);
   }
   if (cbVerbose) {DEBUG ("<  screenHandler\n");}
}

void MainApp::EMOHandler (bool upper, bool triggered)
{
   if (cbVerbose) {DEBUG (">  %s EMOHandler (triggered = %s)\n", upper?"upper":"lower", toCCP (triggered));}
   int index = (upper?1:0);
   emoTriggered [index] = triggered;
   bool anyTriggered = emoTriggered [0] || emoTriggered [1];
   if (anyTriggered != reportedEmo) {
      reportedEmo = anyTriggered;
      liftStateHandler.emoHit (anyTriggered);
#if SIMULATION==1
      sim.setEmoActive (anyTriggered);
#endif

   }
   if (cbVerbose) {DEBUG ("<  EMOHandler\n");}
}


void MainApp::publishFirmwareVersion ()
{

  // firmware/motor/version -- 
  Topic topic (mqtFirmware); topic += mqsMotor;
  topic += mqsVersion;
  String value ("*** Deployed on wrong hardware");
  if (!identity.isFloorController()) {
     // this is indeed a motor controller
     value = compileDT;
  }
  brokerLink.publish (topic, String (compileDT), true);

  DEBUG (">< MainApp:publishFV: topic = %s, value = %s\n", topic.get().c_str (), compileDT);
}


// callbacks are not part of any class
static void anyMessageCB (const String& topic, const String& payload, const size_t size)
{
   if (cbVerbose) {DEBUG (">< Any m CB: '%s' = '%s'\n", topic.c_str (), payload.c_str ());}
}

static void LiftCommandCB (const String &payload, const size_t size)
{
   if (cbVerbose) {DEBUG (">  LiftCommandCB (payload = %s\n", payload.c_str ());}
   LiftCommand liftCommand = stringToLiftCommand (payload);
   switch (liftCommand) {
     case lcGoUp:   liftStateHandler.commandGoUp   (); break;
     case lcGoDown: liftStateHandler.commandGoDown (); break;
     case lcStop:   liftStateHandler.commandStop   (); break;
     case lcNone:   DEBUG ("****liftCommandCB: illegal lift command %s\n", payload);
   }
   if (cbVerbose) {DEBUG ("<  LiftCommandCB\n");}
}

static void upperEmergencyButtonCB (const String& payload, const size_t size)
{
   mainApp.EMOHandler (true, (payload == mqsTriggered));
}

static void lowerEmergencyButtonCB (const String& payload, const size_t size)
{
   mainApp.EMOHandler (false, (payload == mqsTriggered));
}

static void upperScreenCB (const String& payload, const size_t size)
{
   mainApp.screenHandler (true, (payload == mqsTriggered));
}

static void lowerScreenCB (const String& payload, const size_t size)
{
   mainApp.screenHandler (false, (payload == mqsTriggered));
}

void MainApp::subscribeMQTT ()
{
   DEBUG (">  MainApp::subscribeMQTT ()\n");
   bool r = true;
   bool d = (_DEBUG == 1);
   Topic topic (mqtLift); topic += mqsCommand;
   r = r && brokerLink.subscribe (topic, LiftCommandCB, d);
   
   topic.set (mqtIntrusion); topic += mqsUp; topic += mqsStatus;
   r = r && brokerLink.subscribe (topic, upperScreenCB, d);
   
   topic.set (mqtIntrusion); topic += mqsDown; topic += mqsStatus;
   r = r && brokerLink.subscribe (topic, lowerScreenCB, d);

   topic.set (mqtEmergencyButton); topic += mqsUp; topic += mqsStatus;
   r = r && brokerLink.subscribe (topic, upperEmergencyButtonCB, d);

   topic.set (mqtEmergencyButton); topic += mqsDown; topic += mqsStatus;
   r = r && brokerLink.subscribe (topic, lowerEmergencyButtonCB, d);
   
   r = r && brokerLink.subscribe (anyMessageCB, d);

   if (r) {DEBUG ("<  mainApp::subscribeMQTT () = OK\n");}
   else   {DEBUG ("*** mainApp::subscribeMQTT () FAILED\n");}
}

