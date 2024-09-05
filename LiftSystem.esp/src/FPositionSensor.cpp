#include "LiftSystem.hh"
#include "Position.hh"

#include <algorithm>

double FPositionSensor::currentPosition(0.0);

std::vector<FPositionSensor *> FPositionSensor::instances;

FPositionSensor::FPositionSensor(dzn::locator const &locator)
    : skel::FPositionSensor(locator)
    , endstopToMonitor(nullptr)
    , endstopPosition(0.0)
    , previousEndstopState(IEndstop::State::UNKNOWN)
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
  std::for_each(instances.begin(), instances.end(), [](FPositionSensor *s)
                { s->captureEndstop(); });
}

double FPositionSensor::getCurrentPosition()
{
  return currentPosition;
}

bool FPositionSensor::captureEndstop()
{
  if (endstopToMonitor != nullptr)
  {
    ::IEndstop::State currentEndStopState = endstopToMonitor->p_getState();
    if (currentEndStopState != previousEndstopState)
    {
      endstopPosition = currentPosition;
      previousEndstopState = currentEndStopState;
    }
  }
}

double FPositionSensor::capturePosition()
{
  /* TODO */
  
  /**
   * Purpose: to capture the actual position of the list and transform it into meters
   * 
   */ 
}