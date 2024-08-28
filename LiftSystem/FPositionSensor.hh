#ifndef FPOSITIONSENSOR_HH
#define FPOSITIONSENSOR_HH

class FPositionSensor : public skel::FPositionSensor
{
public:
    FPositionSensor(dzn::locator const& locator): skel::FPositionSensor(locator) {};
    virtual ~FPositionSensor() {};

      virtual void p_getCurrentPosition (Position posInM)  { /* TODO*/ };
      virtual void p_getLastEndstopPosition (Position posInM) { /* TODO*/ };
};

#endif