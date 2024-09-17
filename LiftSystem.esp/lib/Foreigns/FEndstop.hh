#ifndef FENDSTOP_HH
#define FENDSTOP_HH

#include <vector>

#include "Location.h"

class FEndstop : public skel::FEndstop
{
public:
  FEndstop(dzn::locator const &locator): skel::FEndstop(locator){};

  virtual ~FEndstop(){};
  virtual ::IEndstop::State p_getState(){};

};

#endif  