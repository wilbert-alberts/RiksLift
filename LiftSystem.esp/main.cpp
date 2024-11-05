//
// main.cpp -- main program for Riks Lift
// this module contains the official setup () and loop () functions. It refers to the methods in class mainApp,
// as defined in mainApp.h. That one contains the real application setup () and loop ().
//
// If _DEBUG == 1, every minute this module reports loop cycle times and the total up time.
//
// BSla, 12 Aug 2024
//

#define _DEBUG 1
#include "debug.h"

#include "timer.h"
#include "mainapp.h"
#include "tester.h"

static uint32_t startTime;
static uint32_t previousTime;

static void loopTime();

static void myDebugPrinter (const char *buffer)
{
   Serial.print (buffer);
}



void setup()
{
   Serial.begin(115200);
   DEBUGAddPrintFunction (myDebugPrinter);
   //DEBUGSetLevel (2); // print errors and warnings
   mainApp.setup();
   tester.setup ();


   startTime = millis (); 
   previousTime = micros();
}


void loop()
{
   mainApp.loop();
   tester.loop ();

   loopTime();
}

static void loopTime()
{
#if _DEBUG == 1
   static int uptime = 0;
   static int loopCount = 0;
   static uint32_t maximumTime = 0;
   static Timer reportTimer(1 MINUTE);

   uint32_t now = micros();
   uint32_t thisLoopTime = now - previousTime;
   previousTime = now;
   if (thisLoopTime > maximumTime)
   {
      maximumTime = thisLoopTime;
   }

   if (reportTimer.hasExpired())
   {
      reportTimer.restart();

      uint32_t m = millis();
      uint32_t expired = m - startTime;

      float usPerLoop = float(expired) * 1000 / loopCount;
      LOG("main loop: uptime = %d minutes; avg loop time = %5.1f us, max loop time = %d us\n",
            uptime, usPerLoop, maximumTime);
      loopCount = 0;
      maximumTime = 0;
      uptime++;
      startTime = m;
   }
   loopCount++;
#endif
}
