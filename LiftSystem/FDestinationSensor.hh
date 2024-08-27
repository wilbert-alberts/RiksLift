#ifndef FDESTINATIONSENSOR_HH
#define FDESTINATIONSENSOR_HH

class FDestinationSensor : public skel::FDestinationSensor
{
public:
    FDestinationSensor(dzn::locator const& locator): skel::FDestinationSensor(locator) {};
    virtual ~FDestinationSensor() {};

      virtual void p_setDestination (Position posInM)  {/* TODO*/};
      virtual void p_cancelDestination () {/* TODO*/};
      virtual void p_getPosition (Position pos)  {/* TODO*/};
};

#endif