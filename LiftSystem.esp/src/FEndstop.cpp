#include "LiftSystem.hh"

FEndstop::FEndstop(dzn::locator const &locator)
    : skel::FEndstop(locator)
{
}

FEndstop::~FEndstop()
{
}

::IEndstop::State FEndstop::p_getState()
{
  /* TODO*/
  return ::IEndstop::State::UNKNOWN;
}