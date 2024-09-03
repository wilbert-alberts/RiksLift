// Generated by dzn code from IPositionSensor.dzn
// import Types.dzn;
#include "IPositionSensor.hh"
#include <dzn/locator.hh>
#include <dzn/runtime.hh>
#include <iterator>
#define STRINGIZING(x) #x
#define STR(x) STRINGIZING (x)
#define LOCATION __FILE__ ":" STR (__LINE__)
IPositionSensor::IPositionSensor (dzn::port::meta const& m)
: dzn_meta (m)
, dzn_share_p (true)
, dzn_label ("")
, dzn_state ()
{}
IPositionSensor::~IPositionSensor ()= default;
void
IPositionSensor::dzn_event (char const* event)
{
  if (!dzn_share_p) return;
  dzn_label = event;
}
void
IPositionSensor::dzn_update_state (dzn::locator const& locator)
{
  if (!dzn_share_p || !dzn_label) return;
  switch (dzn::hash (dzn_label, dzn_state))
    {
      case 765096691u:
      //0:getCurrentPosition
      dzn_state = 1;
      break;
      case 599424005u:
      //0:getLastEndstopPosition
      dzn_state = 1;
      break;
      case 632232382u:
      //1:return
      dzn_state = 0;
      break;
      default: locator.get<dzn::illegal_handler> ().handle (LOCATION);
    }
}
void
IPositionSensor::dzn_check_bindings ()
{
  if (!this->in.getCurrentPosition) throw dzn::binding_error (this->dzn_meta, "in.getCurrentPosition");
  if (!this->in.getLastEndstopPosition) throw dzn::binding_error (this->dzn_meta, "in.getLastEndstopPosition");
}
namespace dzn
{
}
// version 2.18.2
