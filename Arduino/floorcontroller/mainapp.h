//
// mainapp.h -- Riks lift floor controller main application
//
// BSla, 10 aug 2024

#ifndef _MAINAPP_H
#define _MAINAPP_H

#include <Arduino.h>
#include "liftglobals.h"

class MainApp {
 public:
   MainApp () {}
   ~MainApp () {}
   void setup ();
   void loop ();
   void setState (LiftState s); 
   void emButtonHandler (const String &payload, bool upper);
   void screenHandler (const String& payload, bool upper);
   void simScreenHandler (const String& payload, bool upper); 
 private:
   LiftState latestState;
   bool emoTriggered [2];

   void showEmoMessage ();
   void alarm (bool doAlarm);
   void checkScreen ();
   void subscribeMQTT ();
   void publishFirmwareVersion ();
};

extern MainApp mainApp;

#endif