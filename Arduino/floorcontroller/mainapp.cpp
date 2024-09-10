//
// mainapp.cpp -- floor controller main application for Riks Lift
//
// BSla, 10 Aug 2024
//

#define _DEBUG 1
#include "debug.h"
#include "timer.h"

#include "identity.h"
#include "pushbutton.h"
#include "display.h"
#include "buzzer.h"
#include "brokerLink.h"
#include "tester.h"
#include "mainapp.h"

MainApp mainApp;

const char *compileDT = "Floor controller V0.2 " __DATE__ " " __TIME__;


const bool CBVerbose = false;  // should callbacks issue DEBUG statements


void MainApp::setup ()
{
   latestState = lsNone;
   emoTriggered[0] = emoTriggered[1] = false;

   display.setup ();
   brokerLink.setup ();
   pushbutton.setup ();
   emo.setup ();
   screen.setup ();
   buzzer.setup ();

   subscribeMQTT ();
   publishFirmwareVersion ();
   DEBUG ("   MainApp: setup done\n");
}

void MainApp::loop () 
{
   display.loop ();
   brokerLink.loop ();
   pushbutton.loop ();
   emo.loop ();
   screen.loop ();
   buzzer.loop ();

   checkScreen ();
}

void MainApp::setState (LiftState s) 
{
  latestState = s;
  pushbutton.setLED (s);
  display.showState (s);
  DEBUG (">< MainApp::setState: Lift state set to %s\n", liftStateToString (s));
}

void MainApp::publishFirmwareVersion ()
{
  // firmware/up/version -- 
  Topic topic (mqtFirmware); topic += identity.whichFloor(); topic += mqsVersion;

  brokerLink.publish (topic, String (compileDT), true);
  DEBUG (">< MainApp:publishFV: topic = %s, value = %s\n", topic.get().c_str (), compileDT);
}



void MainApp::showEmoMessage ()
{
   if (CBVerbose) DEBUG (">  MainApp::showEmoMessage ()\n");
   int me = identity.isUpperFloor ()? 1:0;
   int other = 1-me;

   bool doMessage = false;
   String message;

   if (emoTriggered [me]) {
      message = "Deze noodstop";
      doMessage = true;
   }
   else if (emoTriggered [other]) {
      message = String("Noodstop ") + String (identity.isUpperFloor ()? "beneden": "boven");
      doMessage = true;
   }

   if (doMessage) {
      message += String (" gedrukt");
      DEBUG ("   MainApp::showEmoMessage: showing message '%s'\n", message.c_str ());
      display.showMessage (message);
   }
   else {
      DEBUG ("   MainApp::showEmoMessage: clearing message\n");
      display.clearMessage ();
   }
   if (CBVerbose) {DEBUG ("<  showEmoMessage\n");}
}

void MainApp::emButtonHandler (const String &payload, bool upper)
{
   DEBUG (">  MainApp %s emoButtonHandler (payload = %s)\n", upper?"upper":"lower", payload.c_str ());
   int which = (upper? 1:0);
   bool t = (payload == "triggered");
   emoTriggered [which] = t;

   showEmoMessage ();
   DEBUG ("<  MainApp::emButtonHandler\n");
}

void MainApp::alarm (bool doAlarm)
{
   static bool alarming = false;
   if (doAlarm != alarming) {
      alarming = doAlarm;
      DEBUG (">< MainApp::alarm %s\n", doAlarm?"on": "off");
      buzzer.buzz (doAlarm);
      pushbutton.alarm (doAlarm);
   }
}

void MainApp::checkScreen ()
{
   bool isUp = identity.isUpperFloor ();
   bool liftIsNotHere = (( isUp && latestState != lsUp  ) || (!isUp && latestState != lsDown)   );
   bool al = screen.isPushed () && liftIsNotHere;
   static bool oldAl = false;
   if (al != oldAl) {
      oldAl = al;
      DEBUG ("   MainApp::CheckScreen: isPushed = %s, lift is %here\n", toCCP(screen.isPushed ()), liftIsNotHere?"NOT ":"");
      alarm (al);
   }
}

void MainApp::screenHandler (const String& payload, bool upper)
{
   if (CBVerbose) {DEBUG (">  %s screenHandler (payload = %s)\n", upper?"upper":"lower", payload.c_str ());}

   if (CBVerbose) {DEBUG ("<  screenHandler\n");}
}


void MainApp::simScreenHandler (const String& payload, bool upper)
{
   if (CBVerbose) {DEBUG (">  %s simScreenHandler (payload = %s)\n", upper?"upper":"lower", payload.c_str ());}

   bool itsme = (upper == identity.isUpperFloor ());
   if (itsme) {
      bool simPushed = false;
      if (payload == "on") simPushed = true;
      screen.setSimPushed (simPushed);
      DEBUG ("   MainApp::simScreenHandler: this %s: set simPushed to %s\n", upper? "upper": "lower", toCCP (simPushed));
   }
   if (CBVerbose) {DEBUG ("<  simScreenHandler\n");}
}


// callbacks are not part of any class
static void anyMessageCB (const String& topic, const String& payload, const size_t size)
{
   if (CBVerbose) {DEBUG (">< Any m CB: '%s' = '%s'\n", topic.c_str (), payload.c_str ());}
}

static void LiftPositionCB (const String &payload, const size_t size)
{
   if (CBVerbose) {DEBUG (">  LiftPositionCB (payload = %s\n", payload.c_str ());}
   LiftState ls = stringToLiftState (payload);
   mainApp.setState (ls);
   if (CBVerbose) {DEBUG ("<  LiftPositionCB\n");}
}

static void upperEmergencyButtonCB (const String& payload, const size_t size)
{
   mainApp.emButtonHandler (payload, true);
}

static void lowerEmergencyButtonCB (const String& payload, const size_t size)
{
   mainApp.emButtonHandler (payload, false);
}

static void upperScreenCB (const String& payload, const size_t size)
{
   mainApp.screenHandler (payload, true);
}

static void lowerScreenCB (const String& payload, const size_t size)
{
   mainApp.screenHandler (payload, false);
}

static void upperSimScreenCB (const String& payload, const size_t size)
{
   mainApp.simScreenHandler (payload, true);
}

static void lowerSimScreenCB (const String& payload, const size_t size)
{
   mainApp.simScreenHandler (payload, false);
}

static void moveTimeCB (const String& payload, const size_t size)
{
   int seconds = payload.toInt (); 
   display.setMoveTime (seconds);
}


void MainApp::subscribeMQTT ()
{
   DEBUG (">  MainApp::subscribeMQTT ()\n");
   bool r = true;
   bool d = (_DEBUG == 1);
   Topic topic (mqtLift); topic += mqsStatus;
   r = r && brokerLink.subscribe (topic, LiftPositionCB, d);               // lift position
   
   topic.set (mqtEmergencyButton); topic += mqsUp; topic += mqsStatus;
   r = r && brokerLink.subscribe (topic, upperEmergencyButtonCB, d);       // emergency button

   topic.set (mqtEmergencyButton); topic += mqsDown; topic += mqsStatus;
   r = r && brokerLink.subscribe (topic, lowerEmergencyButtonCB, d);
   
   topic.set (mqtIntrusion); topic += mqsUp; topic += mqsStatus;
   r = r && brokerLink.subscribe (topic, upperScreenCB, d);
   
   topic.set (mqtIntrusion); topic += mqsDown; topic += mqsStatus;
   r = r && brokerLink.subscribe (topic, lowerScreenCB, d);

   topic.set ("simScreen"); topic += mqsUp; topic += mqsStatus;
   r = r && brokerLink.subscribe (topic, upperSimScreenCB, d);
   
   topic.set ("simScreen"); topic += mqsDown; topic += mqsStatus;
   r = r && brokerLink.subscribe (topic, lowerSimScreenCB, d);

   topic.set (mqtLift); topic += mqsXpMoveTime;                           // expected move time
   r = r && brokerLink.subscribe (topic, moveTimeCB, d);

   r = r && brokerLink.subscribe (anyMessageCB, d);

   if (r) {DEBUG ("<  mainApp::subscribeMQTT () = OK\n");}
   else   {DEBUG ("*** mainApp::subscribeMQTT () FAILED\n");}
}
