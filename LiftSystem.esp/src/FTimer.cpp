#include <Arduino.h>
#include <algorithm>

#include "LiftSystem.hh"

// Collection of pointers to FTimer in order to trigger all of them
// during loop

std::vector<FTimer *> FTimer::instances;

FTimer::FTimer(dzn::locator const &locator)
    : skel::FTimer(locator), armed(false), startTimeInMs(0), delayTimeInMs(0)
{
  instances.push_back(this);
};

FTimer::~FTimer()
{
  auto pos = std::find(instances.begin(), instances.end(), this);
  if (pos != instances.end())
    instances.erase(pos);
};

void FTimer::p_setTimer(Delay delayInS)
{
  delayTimeInMs = (unsigned long)(1000.0 * delayInS.getDelay());
  startTimeInMs = millis();
  armed = true;
}

void FTimer::p_cancelTimer()
{
  armed = false;
}

void FTimer::checkElapsed()
{
  if (armed)
  {
    if (millis() - startTimeInMs > delayTimeInMs)
    {
      this->p.out.timerElapsed();
    }
  }
}

void FTimer::loop()
{
  std::for_each(instances.begin(), instances.end(), 
    [](FTimer *t) { t->checkElapsed(); });
}