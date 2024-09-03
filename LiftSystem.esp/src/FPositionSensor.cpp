#include "LiftSystem.hh"
#include "Position.hh"

double FPositionSensor::currentPosition(0.0);
double FPositionSensor::endstopPosition(0.0);
bool FPositionSensor::previousEndstopState(captureEndstop());

FPositionSensor::FPositionSensor(dzn::locator const &locator)
    : skel::FPositionSensor(locator)
{
}

FPositionSensor::~FPositionSensor()
{
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
  bool currentEndStopState = captureEndstop();
  if (currentEndStopState != previousEndstopState)
  {
    endstopPosition = currentPosition;
    previousEndstopState = currentEndStopState;
  }
}

double FPositionSensor::getCurrentPosition()
{
  return currentPosition;
}

bool FPositionSensor::captureEndstop()
{
  /* TODO */
}

double FPositionSensor::capturePosition()
{
  /* TODO */
}