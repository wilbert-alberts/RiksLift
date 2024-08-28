#ifndef FPOSITIONSENSOR_HH
#define FPOSITIONSENSOR_HH

class FPositionSensor : public skel::FPositionSensor
{
public:
    FPositionSensor(dzn::locator const& locator);
    virtual ~FPositionSensor();

      virtual void p_getCurrentPosition (Position posInM);
      virtual void p_getLastEndstopPosition (Position posInM) ;
};

#endif