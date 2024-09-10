//
// mainapp.h -- Riks lift floor controller main application
//
// BSla, 10 aug 2024

#ifndef _MAINAPP_H
#define _MAINAPP_H

#include <Arduino.h>
#include "liftglobals.h"
#include "liftState.h"

class MainApp {
 private:
   void publishState ();
 public:
   MainApp () {}
   ~MainApp () {}
   void setup ();
   void loop ();
   void EMOHandler    (bool upper, bool triggered); // linked to MQTT callbacks
   void screenHandler (bool upper, bool triggered);
   void publishState (LiftState ls) {publishedState = ls; publishState ();}
 private:
   bool emoTriggered [2];
   bool reportedEmo;
   bool screenTriggered [2];
   bool reportedScreen;
   LiftState publishedState;
   Timer publishStateTimer;
   void publishFirmwareVersion ();
   bool anyEmoActive ();
   void subscribeMQTT ();
};

extern MainApp mainApp;

#endif