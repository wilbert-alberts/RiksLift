#ifndef FDESTINATIONSENSOR_HH
#define FDESTINATIONSENSOR_HH

class FDestinationSensor : public skel::FDestinationSensor
{
public:
  FDestinationSensor(dzn::locator const &locator);
  virtual ~FDestinationSensor();

  virtual void p_setDestination(Position posInM);
  virtual void p_cancelDestination();
  virtual void p_getPosition(Position pos);
};

#endif