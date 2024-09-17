#include "LiftSystem.hh"

#include <algorithm>

std::vector<FEndstop *> FEndstop::instances;

FEndstop::FEndstop(dzn::locator const &locator)
    : skel::FEndstop(locator)
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

void FEndstop::checkTransition() {
  ::IEndstop::State newState = readState();
  if (newState != lastState) {
    if (newState == ::IEndstop::State::INSIDE) {
      this->p.out.entering();
    }
    if (newState == ::IEndstop::State::OUTSIDE) {
      this->p.out.exiting();
    }

    lastState = newState;
  }
}

void FEndstop::setLocation(Location l) {
  /* TODO */

  // Purpose: To configure location of endstop.
  //          Affects behavior or readState()
}

::IEndstop::State FEndstop::readState() {
  /* TODO */

  /** 
   * Purpose: To capture the state of the endstop this object is connected to.
   *          The state of the endstop if likely communicated via MQTT so this
   *          class wil have to start listening to MQTT message that corresponds
   *          to the state of the endstop hardware that belongs to the 
   *          corresponding floor.
  */
}