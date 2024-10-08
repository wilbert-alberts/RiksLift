#include "LiftSystem.hh"

#include <algorithm>

std::vector<FEndstop *> FEndstop::instances;

FEndstop::FEndstop(dzn::locator const &locator)
    : skel::FEndstop(locator), lastState(::IEndstop::State::UNKNOWN), thisEndstop(nullptr)
{
  instances.push_back(this);
}

FEndstop::~FEndstop()
{
  auto pos = std::find(instances.begin(), instances.end(), this);
  if (pos != instances.end())
  {
    instances.erase(pos);
  }
}

::IEndstop::State FEndstop::p_getState()
{
  return lastState;
}

void FEndstop::loop()
{
  std::for_each(instances.begin(), instances.end(), [](FEndstop *e)
                { e->checkTransition(); });
}

void FEndstop::checkTransition()
{
  if (lastState == ::IEndstop::State::UNKNOWN)
  {
    lastState = readState();
    DEBUG("FEndstop::checkTransition: lastState changed from UNKNOWN (%d) to %d\n", ::IEndstop::State::UNKNOWN, lastState);
  }

  ::IEndstop::State newState = readState();
  if (newState != lastState)
  {
    DEBUG("FEndstop::checkTransition: lastState changed from %d to %d\n", lastState, newState);
    if (newState == ::IEndstop::State::INSIDE)
    {
      DEBUG("FEndstop::checkTransition: entering\n");
      this->p.out.entering();
    }
    if (newState == ::IEndstop::State::OUTSIDE)
    {
      DEBUG("FEndstop::checkTransition: exiting\n");
      this->p.out.exiting();
    }

    lastState = newState;
  }
}

void FEndstop::setLocation(Location l)
{
  // Purpose: To configure location of endstop.
  //          Affects behavior of readState()
  if (l == UPPER_FLOOR)
    thisEndstop = &upperEndstop;
  else if (l == LOWER_FLOOR)
    thisEndstop = &lowerEndstop;
}

::IEndstop::State FEndstop::readState()
{
  /**
   * Purpose: To capture the state of the endstop this object is connected to.

  */
  ::IEndstop::State st = thisEndstop->reached() ? ::IEndstop::State::INSIDE : ::IEndstop::State::OUTSIDE;
  return st;
}
