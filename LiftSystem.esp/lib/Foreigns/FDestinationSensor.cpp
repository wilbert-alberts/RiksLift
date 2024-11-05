#include "LiftSystem.hh"

#include <algorithm>

#include "FPositionSensor.hh"

#define _DEBUG 1
#include "debug.h"

std::vector<FDestinationSensor *> FDestinationSensor::instances;

// Comparision accuracy 1 mm
double FDestinationSensor::eps(1.0e-2);

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
  DEBUG(">  FDestinationSensor::p_setDestination(%f)\n", posInM.getPosition());
  destination = posInM.getPosition();
  armed = true;
  DEBUG("<  FDestinationSensor::p_setDestination()\n" );
}

void FDestinationSensor::p_cancelDestination()
{
  armed = false;
}

void FDestinationSensor::checkArrived(double currentPosition)
{
  if (armed)
  {
    static uint32_t daarnet = 0;
    uint32_t now = millis ();
    if (now - daarnet > 1000) {
      daarnet = now;
      DEBUG (">< FDestinationSensor::checkArrived (current pos = %f), dest = %f\n", currentPosition, destination);
    }

    if (fabs(currentPosition - destination) < eps)
    {
      DEBUG(">< FDestinationSensor::checkArrived(): arrived\n, destination was: %f, current is: %f\n", destination, currentPosition);
      armed = false;
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