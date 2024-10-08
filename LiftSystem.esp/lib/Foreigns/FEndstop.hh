#ifndef FENDSTOP_HH
#define FENDSTOP_HH

#include <vector>

#include "Location.h"
#include "endstop.h"

class FEndstop : public skel::FEndstop
{
public:
  FEndstop(dzn::locator const &locator);
  void setLocation(Location location);

  virtual ~FEndstop();
  virtual ::IEndstop::State p_getState();

  static void loop();

private:
  ::IEndstop::State lastState;
  void checkTransition();
  ::IEndstop::State readState();

  Endstop *thisEndstop;

  static std::vector<FEndstop*> instances;

};

#endif  
