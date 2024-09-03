// Generated by dzn code from ITimer.dzn
#include <dzn/runtime.hh>
namespace dzn
{
  struct locator;
  struct runtime;
}
#include <iostream>
#include <vector>
#include <map>
#ifndef ITIMER_HH
#define ITIMER_HH
struct ITimer
{
  enum struct State
    {
      IDLE,ARMED
    };
  dzn::port::meta dzn_meta;
  struct
    {
      dzn::in::event<void (Delay delayInS)> setTimer;
      dzn::in::event<void ()> cancelTimer;
    } in;
  struct
    {
      dzn::out::event<void ()> timerElapsed;
    } out;
  bool dzn_share_p;
  char const* dzn_label;
  int dzn_state;
  ::ITimer::State state;
  ITimer (dzn::port::meta const& m);
  template <typename Component>
  ITimer (dzn::port::meta const& m, Component* that)
  : dzn_meta (m)
  , dzn_share_p (true)
  , dzn_label ("")
  , dzn_state ()
  , state (::ITimer::State::IDLE)
    {
      in.setTimer.set (that, this, "setTimer");
      in.cancelTimer.set (that, this, "cancelTimer");
      out.timerElapsed.set (that, this, "timerElapsed");
    }
  virtual ~ITimer ();
  void dzn_event (char const* event);
  void dzn_update_state (dzn::locator const& locator);
  void dzn_check_bindings ();
};
namespace dzn
{
  inline void connect (::ITimer& provide, ::ITimer& require)
    {
      provide.out.timerElapsed = require.out.timerElapsed;
      require.in.setTimer = provide.in.setTimer;
      require.in.cancelTimer = provide.in.cancelTimer;
      provide.dzn_meta.require = require.dzn_meta.require;
      require.dzn_meta.provide = provide.dzn_meta.provide;
      provide.dzn_share_p = require.dzn_share_p = provide.dzn_share_p && require.dzn_share_p;
    }
}
#endif // ITIMER_HH
// version 2.18.2
