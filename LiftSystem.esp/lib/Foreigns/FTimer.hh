#ifndef FTIMER_HH
#define FTIMER_HH

#include <vector>

class FTimer : public skel::FTimer
{
public:
  FTimer(dzn::locator const &locator);
  virtual ~FTimer();

  virtual void p_setTimer(double delayInS);
  virtual void p_cancelTimer();

  void checkElapsed();

  static void loop();

private:
  static std::vector<FTimer*> instances;

  bool          armed;
  unsigned long startTimeInMs;
  unsigned long delayTimeInMs;

};

#endif