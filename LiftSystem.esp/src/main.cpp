
#include <Arduino.h>
#include "LiftSystem.hh"
#include "FMoveUpDown.h"
#include "dzn/runtime.hh"
#include "dzn/locator.hh"
#include "FTimer.hh"
#include "FPositionSensor.hh"
#include "FEndstop.hh"
#include "brokerlink.h"

#include "ForeignPlanner.hh"
#include "FPlanner.hh"
#include "ForeignMotor.hh"
#include "FMotor.hh"

#include "hsi.h"
#include "liftencoder.h"
#include "simulation.h"
#include "vevorVFD.h"

#define _DEBUG 1
#include "debug.h"

dzn::locator loc;
dzn::runtime rt;
std::unique_ptr<LiftSystem> liftSystem;
std::unique_ptr<FMoveUpDown> fMoveUpDown;

static uint32_t startTime;
static uint32_t previousTime;

static void loopTime();

static void myDebugPrinter (const char *buffer)
{
   Serial.print (buffer);
}



void setup() {
  Serial.begin(115200);
  DEBUGAddPrintFunction (myDebugPrinter);

  DEBUG ("setup\n");

  lowerEndstop.setup (GPIO_LOWER_ENDSTOP);
  upperEndstop.setup (GPIO_UPPER_ENDSTOP);
  liftEncoder.setup ();
  sim.setup ();
  vevorVFD.setup ();
  brokerLink.setup();

  loc.set(rt);
  liftSystem = std::unique_ptr<LiftSystem>(new LiftSystem(loc));
  fMoveUpDown = std::unique_ptr<FMoveUpDown>( new FMoveUpDown());
  liftSystem->fLog.setComponentID("mudArmor");
  fMoveUpDown->setSystem(liftSystem.get ());
  liftSystem->huEndstop.setLocation(UPPER_FLOOR);
  liftSystem->hdEndstop.setLocation(LOWER_FLOOR);
  liftSystem->huPositionSensor.setEndstopToMonitor(UPPER_FLOOR, &liftSystem->huEndstop);
  liftSystem->hdPositionSensor.setEndstopToMonitor(LOWER_FLOOR, &liftSystem->hdEndstop);
  fMoveUpDown->connect();
}

void loop() {  
  static uint32_t loopCount = 0;
  brokerLink.loop();
  lowerEndstop.loop ();
  upperEndstop.loop ();
  sim.loop ();
  vevorVFD.loop ();
  FTimer::loop();
  FPositionSensor::loop();
  FDestinationSensor::loop();
  FEndstop::loop();
  //if (++loopCount %10000 == 0)  DEBUG(".");

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

