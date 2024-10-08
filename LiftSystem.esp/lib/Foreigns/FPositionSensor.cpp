#include "LiftSystem.hh"
#include "Position.hh"
#include "liftencoder.h"
#include "brokerlink.h"

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

void FPositionSensor::setEndstopToMonitor(FEndstop *endstop)
{
  endstopToMonitor = endstop;
}

void FPositionSensor::p_getCurrentPosition(Position posInM)
{
  posInM.setPosition(FPositionSensor::currentPosition);
}

void FPositionSensor::p_getLastEndstopPosition(Position posInM)
{
  posInM.setPosition(FPositionSensor::endstopPosition);
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
  bool r = false;
  if (endstopToMonitor != nullptr)
  {
    ::IEndstop::State currentEndStopState = endstopToMonitor->p_getState();
    if (currentEndStopState != previousEndstopState)
    {
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