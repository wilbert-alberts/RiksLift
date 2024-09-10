//
// floorcontroller.ino -- floor controller for Riks Lift
//
// BSla, 7 Jul 2024
//

#define _DEBUG 1
#include "debug.h"
#include "timer.h"

#include "mainapp.h"
#include "tester.h"

static const uint32_t MainLoopReportInterval   (1 MINUTE);

static void loopTime ();

void setup ()
{
   Serial.begin(115200);
   mainApp.setup ();
   //tester.setup ();
   // DEBUG ("   Main: setup done\n");
}

void loop () 
{
   mainApp.loop ();
  // tester.loop ();
   loopTime ();
}

static void loopTime ()
{
#if _DEBUG == 1  
   static int uptime = 0;
   static int loopCount = 0;
   static uint32_t maximumMicros = 0;
   static Timer reportTimer;
   static uint32_t startMicros;
   static uint32_t previousMicros;

   uint32_t nowMicros  = micros ();

   if (!reportTimer.isActive () && !reportTimer.hasExpired ()) {
      reportTimer.start (MainLoopReportInterval);
      startMicros = previousMicros = nowMicros;
   }


   uint32_t thisLoopMicros = nowMicros - previousMicros;
   previousMicros = nowMicros;

   if (thisLoopMicros > maximumMicros) {
      maximumMicros = thisLoopMicros;
   }

   if (reportTimer.hasExpired ()) {
      reportTimer.restart ();

      uint32_t expired = nowMicros - startMicros;

      float usPerLoop = float (expired) / loopCount;
      DEBUG ("main loop: uptime = %d minutes; avg loop time = %5.1f us, max loop time = %d us\n", 
	             uptime, usPerLoop, maximumMicros);
      loopCount   = maximumMicros = 0;
      uptime++;
      startMicros  = nowMicros;
   }
   loopCount++;
#endif
}
