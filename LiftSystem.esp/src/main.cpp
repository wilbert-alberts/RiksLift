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

void setup() {
  Serial.begin(115200);
  DEBUG("> setup\n");

  lowerEndstop.setup (GPIO_LOWER_ENDSTOP);
  upperEndstop.setup (GPIO_UPPER_ENDSTOP);
  liftEncoder.setup ();
  sim.setup ();
  vevorVFD.setup ();

  loc.set(rt);
  liftSystem = std::unique_ptr<LiftSystem>(new LiftSystem(loc));
  fMoveUpDown = std::unique_ptr<FMoveUpDown>( new FMoveUpDown());

  brokerLink.setup();

  liftSystem->fLog.setComponentID("mudArmor");
  fMoveUpDown->setSystem(liftSystem.get());
  liftSystem->huEndstop.setLocation(UPPER_FLOOR);
  liftSystem->hdEndstop.setLocation(LOWER_FLOOR);
  liftSystem->huPositionSensor.setEndstopToMonitor(&liftSystem->huEndstop);
  liftSystem->hdPositionSensor.setEndstopToMonitor(&liftSystem->hdEndstop);
  
  fMoveUpDown->connect();
}

void loop() {
  static uint32_t loopCount = 0;
//  DEBUG("> loop\n");
  brokerLink.loop();
  lowerEndstop.loop ();
  upperEndstop.loop ();
  sim.loop ();
  vevorVFD.loop ();
  FTimer::loop();
  FPositionSensor::loop();
  FDestinationSensor::loop();
  FEndstop::loop();
//  delay(1000);
  if (++loopCount %10000 == 0)  DEBUG(".");
}
