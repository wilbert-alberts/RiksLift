#ifndef FTIMER_HH
#define FTIMER_HH

class FTimer : public skel::FTimer
{
public:
    FTimer(dzn::locator const& locator);
    virtual ~FTimer();

      virtual void p_setTimer (Delay delayInS) ;
      virtual void p_cancelTimer () ;
};

#endif