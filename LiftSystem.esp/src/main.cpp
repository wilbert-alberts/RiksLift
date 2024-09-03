#include <Arduino.h>
#include "LiftSystem.hh"
#include "FMoveUpDown.h"
#include "dzn/runtime.hh"
#include "dzn/locator.hh"
#include "FTimer.hh"
#include "FPositionSensor.hh"

dzn::locator loc;
dzn::runtime rt;

LiftSystem liftSystem(loc);
FMoveUpDown fMoveUpDown;

void setup() {
  loc.set(rt);

  liftSystem.fLog.setComponentID("mudArmor");
  fMoveUpDown.setSystem(&liftSystem);
  
}

void loop() {
  FTimer::loop();
  FPositionSensor::loop();
  FDestinationSensor::loop();
}
