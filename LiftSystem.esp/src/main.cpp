#include <Arduino.h>
#include "LiftSystem.hh"
#include "FMoveUpDown.h"
#include "dzn/runtime.hh"
#include "dzn/locator.hh"
#include "FTimer.hh"
#include "FPositionSensor.hh"
#include "FEndstop.hh"

dzn::locator loc;
dzn::runtime rt;

LiftSystem liftSystem(loc);
FMoveUpDown fMoveUpDown;

void setup() {
  loc.set(rt);

  liftSystem.fLog.setComponentID("mudArmor");
  fMoveUpDown.setSystem(&liftSystem);
  liftSystem.huEndstop.setLocation(UPPER_FLOOR);
  liftSystem.hdEndstop.setLocation(LOWER_FLOOR);
  liftSystem.huPositionSensor.setEndstopToMonitor(&liftSystem.huEndstop);
  liftSystem.hdPositionSensor.setEndstopToMonitor(&liftSystem.hdEndstop);
  
}

void loop() {
  FTimer::loop();
  FPositionSensor::loop();
  FDestinationSensor::loop();
  FEndstop::loop();
}
