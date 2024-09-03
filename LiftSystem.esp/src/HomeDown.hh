// Generated by dzn code from HomeDown.dzn
#include <dzn/runtime.hh>
namespace dzn
{
  struct locator;
  struct runtime;
}
#include <iostream>
#include <vector>
#include <map>
#include "ITimer.hh"
#include "IMotor.hh"
#include "IDestinationSensor.hh"
#include "IPlanner.hh"
#include "IPositionSensor.hh"
#include "IMoveDown.hh"
#include "IEndstop.hh"
#ifndef HOMEDOWN_HH
#define HOMEDOWN_HH
#include "IMoveDown.hh"
#include "ITimer.hh"
#include "IEndstop.hh"
#include "IMotor.hh"
#include "IPositionSensor.hh"
#include "IPlanner.hh"
struct HomeDown: public dzn::component
{
  enum struct State
    {
      IDLE,MOVING_FAST_IN,STOPPING_FAST_IN,MOVING_SLOW_OUT,MOVING_SLOW_EXTRA_OUT,STOPPING_SLOW_OUT,MOVING_SLOW_IN,STOPPING_SLOW_IN,FAILING_STOPPING,ABORTING
    };
  dzn::meta dzn_meta;
  dzn::runtime& dzn_runtime;
  dzn::locator const& dzn_locator;
  ::HomeDown::State state;
  Delay delay;
  Position position;
  ::IEndstop::State es;
  ::IEndstop::State* dzn_reply_IEndstop_State;
  std::function<void ()>* dzn_out_p;
  ::IMoveDown p;
  ::ITimer timer;
  ::IEndstop endstop;
  ::IMotor motor;
  ::IPositionSensor positionSensor;
  ::IPlanner planner;
  HomeDown (dzn::locator const& locator);
  private:
  void p_moveDown ();
  void p_abort ();
  void timer_timerElapsed ();
  void endstop_entering ();
  void endstop_exiting ();
};
#endif // HOMEDOWN_HH
// version 2.18.2
