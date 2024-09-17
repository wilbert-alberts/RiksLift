#ifndef FDESTINATIONSENSOR_HH
#define FDESTINATIONSENSOR_HH

#include <vector>


class FDestinationSensor : public skel::FDestinationSensor
{
public:
  FDestinationSensor(dzn::locator const &locator): skel::FDestinationSensor(locator){};
  virtual ~FDestinationSensor(){};

  virtual void p_setDestination(Position posInM) {};
  virtual void p_cancelDestination() {};

private:
};

#endif