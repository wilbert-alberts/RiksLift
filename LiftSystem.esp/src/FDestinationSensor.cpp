#include "LiftSystem.hh"

#include <algorithm>

#include "FPositionSensor.hh"

std::vector<FDestinationSensor *> FDestinationSensor::instances;

// Comparision accuracy 1 mm
double FDestinationSensor::eps(1.0e-3);

FDestinationSensor::FDestinationSensor(dzn::locator const &locator)
    : skel::FDestinationSensor(locator), armed(false), destination(0.0)
{
  instances.push_back(this);
};

FDestinationSensor::~FDestinationSensor()
{
  auto pos = std::find(instances.begin(), instances.end(), this);
  if (pos != instances.end())
    instances.erase(pos);
};

void FDestinationSensor::p_setDestination(Position posInM)
{
  destination = posInM.getPosition();
  armed = true;
}

void FDestinationSensor::p_cancelDestination()
{
  armed = false;
}

// void FDestinationSensor::p_getPosition(Position pos)
// {
//   /* TODO: check whether needed! Should be retrieved via FPositionSensor */
// }

void FDestinationSensor::checkArrived(double currentPosition)
{
  if (armed)
  {
    if (abs(currentPosition - destination) < eps)
    {
      this->p.out.destinationReached();
    }
  }
}

void FDestinationSensor::loop()
{
  double currentPosition = FPositionSensor::getCurrentPosition();
  std::for_each(instances.begin(), instances.end(), [currentPosition](FDestinationSensor *s)
                { s->checkArrived(currentPosition); });
}