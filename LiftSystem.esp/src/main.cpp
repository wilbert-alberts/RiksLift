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

#define _DEBUG 1
#include "debug.h"

dzn::locator loc;
dzn::runtime rt;
std::unique_ptr<LiftSystem> liftSystem;

void setup() {
  Serial.begin(115200);
  DEBUG("> setup\n");

  loc.set(rt);
  liftSystem = std::unique_ptr<LiftSystem>(new LiftSystem(loc));

  brokerLink.setup();
  // FMoveUpDown fMoveUpDown;

  // liftSystem.fLog.setComponentID("mudArmor");
  // fMoveUpDown.setSystem(&liftSystem);
  // liftSystem.huEndstop.setLocation(UPPER_FLOOR);
  // liftSystem.hdEndstop.setLocation(LOWER_FLOOR);
  // liftSystem.huPositionSensor.setEndstopToMonitor(&liftSystem.huEndstop);
  // liftSystem.hdPositionSensor.setEndstopToMonitor(&liftSystem.hdEndstop);
  
  // fMoveUpDown.connect();
}

void loop() {
  DEBUG("> loop\n");
  brokerLink.loop();
  // FTimer::loop();
  // FPositionSensor::loop();
  // FDestinationSensor::loop();
  // FEndstop::loop();
  delay(1000);
  DEBUG("< loop\n");
}
