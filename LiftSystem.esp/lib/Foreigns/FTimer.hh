#ifndef FTIMER_HH
#define FTIMER_HH

#include <vector>

class FTimer : public skel::FTimer
{
public:
  FTimer(dzn::locator const &locator): skel::FTimer(locator){};
  virtual ~FTimer(){};

  virtual void p_setTimer(Delay delayInS){};
  virtual void p_cancelTimer(){};

};

#endif