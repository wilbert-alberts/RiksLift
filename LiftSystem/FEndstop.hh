#ifndef FENDSTOP_HH
#define FENDSTOP_HH

class FEndstop : public skel::FEndstop
{
public:
  FEndstop(dzn::locator const &locator) : skel::FEndstop(locator) {};
  virtual ~FEndstop() {};

  virtual ::IEndstop::State p_getState() { /* TODO*/ return ::IEndstop::State::UNKNOWN; };
};

#endif