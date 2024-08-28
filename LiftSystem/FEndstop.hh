#ifndef FENDSTOP_HH
#define FENDSTOP_HH

class FEndstop : public skel::FEndstop
{
public:
  FEndstop(dzn::locator const &locator);
  virtual ~FEndstop();
  virtual ::IEndstop::State p_getState();
};

#endif  