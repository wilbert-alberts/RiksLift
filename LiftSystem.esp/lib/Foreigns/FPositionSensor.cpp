#include "LiftSystem.hh"
#include "liftencoder.h"
#include "brokerlink.h"

#define _DEBUG 1
#include "debug.h"

#include <algorithm>

double FPositionSensor::currentPosition(0.0);

std::vector<FPositionSensor *> FPositionSensor::instances;

void reportPosition(double pos);

FPositionSensor::FPositionSensor(dzn::locator const &locator)
    : skel::FPositionSensor(locator), endstopToMonitor(nullptr), endstopPosition(0.0), previousEndstopState(IEndstop::State::UNKNOWN)
{
  instances.push_back(this);
}

FPositionSensor::~FPositionSensor()
{
  auto pos = std::find(instances.begin(), instances.end(), this);
  if (pos != instances.end())
  {
    instances.erase(pos);
  }
}

void FPositionSensor::setEndstopToMonitor(Location location, FEndstop *endstop)
{
  this->location = location;
  endstopToMonitor = endstop;
}

void FPositionSensor::p_getCurrentPosition(double* posInM)
{
  DEBUG(">  FPositionSensor::p_getCurrentPosition ()");
  *posInM = FPositionSensor::currentPosition;
  DEBUG("<>>  FPositionSensor::p_getCurrentPosition (posInM = %f\n)", *posInM);
}

void FPositionSensor::p_getLastEndstopPosition(double* posInM)
{
  DEBUG(">  FPositionSensor::p_getLastEndstopPosition()\n");
  *posInM = FPositionSensor::endstopPosition;
  DEBUG("<  FPositionSensor::p_getLastEndstopPosition (posInM = %f)\n", *posInM);
}

void FPositionSensor::loop()
{
  currentPosition = capturePosition();
  reportPosition(currentPosition);
  std::for_each(instances.begin(), instances.end(), [](FPositionSensor *s)
                { s->captureEndstop(); });
}

void reportPosition(double pos)
{
  static uint32_t interval = 1000;
  static bool done = false;
  static String topic = String("positionSensor/status");
  String posStr = String(pos);

  if ((!done) && (millis() % interval == 0))
  {
    brokerLink.publish(topic, posStr, false);
    done = true;
  }
  else
  {
    done = false;
  }
}

double FPositionSensor::getCurrentPosition()
{
  return currentPosition;
}

bool FPositionSensor::captureEndstop()
{
  static uint32_t prevReport = 0;
  bool r = false;
  if (endstopToMonitor != nullptr)
  {

    ::IEndstop::State currentEndStopState = endstopToMonitor->p_getState();
    // if (millis () - prevReport > 500 && location == Location::UPPER_FLOOR) {
    //     prevReport  = millis ();
    //     DEBUG (">< FPositionSensor::captureEndstop: current endstop state = %d\n", currentEndStopState);
    // }

    if (currentEndStopState != previousEndstopState)
    {
      DEBUG("   FPositionSensor::captureEndstop state change; previous state = %d, new state = %d, at position %f\n",
            previousEndstopState, currentEndStopState, currentPosition);
      endstopPosition = currentPosition;
      previousEndstopState = currentEndStopState;
      r = true;
    }
  }
  return r;
}

double FPositionSensor::capturePosition()
{
  /**
   * Purpose: to capture the actual position of the lift and transform it into meters
   *
   */
  return liftEncoder.getPosition();
}