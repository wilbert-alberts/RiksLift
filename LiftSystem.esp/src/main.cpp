#include <Arduino.h>
#include <sstream>
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

std::string debugBuffer;
std::stringstream debugStream(debugBuffer);

void setup()
{
  Serial.begin(115200);
  DEBUG("setup\n");

  lowerEndstop.setup(GPIO_LOWER_ENDSTOP);
  upperEndstop.setup(GPIO_UPPER_ENDSTOP);
  liftEncoder.setup();
  sim.setup();
  vevorVFD.setup();
  brokerLink.setup();

  loc.set(rt);
  loc.set(debugStream);

  liftSystem = std::unique_ptr<LiftSystem>(new LiftSystem(loc));
  fMoveUpDown = std::unique_ptr<FMoveUpDown>(new FMoveUpDown());
  liftSystem->fLog.setComponentID("mudArmor");
  fMoveUpDown->setSystem(liftSystem.get());
  liftSystem->huEndstop.setLocation(UPPER_FLOOR);
  liftSystem->hdEndstop.setLocation(LOWER_FLOOR);
  liftSystem->huPositionSensor.setEndstopToMonitor(UPPER_FLOOR, &liftSystem->huEndstop);
  liftSystem->hdPositionSensor.setEndstopToMonitor(LOWER_FLOOR, &liftSystem->hdEndstop);
  fMoveUpDown->connect();
}

void loop()
{
  static uint32_t loopCount = 0;
  brokerLink.loop();
  lowerEndstop.loop();
  upperEndstop.loop();
  sim.loop();
  vevorVFD.loop();
  FTimer::loop();
  FPositionSensor::loop();
  FDestinationSensor::loop();
  FEndstop::loop();
  if (++loopCount % 10000 == 0)
    DEBUG(".");
  if (++loopCount % 10000 == 0)
  {
    DEBUG("-----------------------debugBuffer-----------------------\n");
    DEBUG(debugBuffer.c_str());
    DEBUG("-----------------------debugBuffer-----------------------\n");
    debugBuffer = "";
  }
}
