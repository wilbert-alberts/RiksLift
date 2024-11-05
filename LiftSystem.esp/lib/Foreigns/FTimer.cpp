#include <Arduino.h>
#include <algorithm>

#define _DEBUG 0
#include "debug.h"

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
  DEBUG ("> pSetTimer (delayInS = %d)\n", delayInS.getDelay ());
  delayTimeInMs = (unsigned long)(1000.0 * delayInS.getDelay());
  startTimeInMs = millis();
  armed = true;
  DEBUG ("< pSetTimer: startTimeInMs = %d\n", startTimeInMs);
}

void FTimer::p_cancelTimer()
{
  DEBUG (">< FTimer::p_cancelTimer ((delayInS = %d))\n", delayTimeInMs/1000);
  armed = false;
}

void FTimer::checkElapsed()
{
  if (armed)
  {
    uint32_t elapsed = millis () - startTimeInMs;
    DEBUG (">  FTimer::checkElapsed (): elapsed = %d, delay = %d\n", elapsed, delayTimeInMs);
    if (elapsed > delayTimeInMs)
    {
      DEBUG ("   FTimer:checkElapsed: ELAPSED\n");
      armed = false;
      this->p.out.timerElapsed();
    }
  }
}

void FTimer::loop()
{
  std::for_each(instances.begin(), instances.end(), 
    [](FTimer *t) { t->checkElapsed(); });
}