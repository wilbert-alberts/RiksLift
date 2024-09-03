// Generated by dzn code from ForeignPlanner.dzn
#include "ForeignPlanner.hh"
#include <dzn/locator.hh>
#include <dzn/runtime.hh>
#include <iterator>
#define STRINGIZING(x) #x
#define STR(x) STRINGIZING (x)
#define LOCATION __FILE__ ":" STR (__LINE__)
namespace skel
{
  FPlanner::FPlanner (dzn::locator const& locator)
  : dzn_meta (  {"FPlanner","FPlanner",0,  {},  {},  {[this] ()
            {
              p.dzn_check_bindings ();
            }}})
  , dzn_runtime (locator.get<dzn::runtime> ())
  , dzn_locator (locator)
  , p (  {  {"p",&p,this,&dzn_meta},  {"p",0,0,0}},this)
    {
      this->p.in.getMoveUpFastPlanning = [this] (Position current, Delay d, Position p)
        {
          this->dzn_out_p = &this->p.in.getMoveUpFastPlanning.dzn_out_binding;
          this->p_getMoveUpFastPlanning (current, d, p);
        };
      this->p.in.getMoveUpSlowPlanning = [this] (Position current, Delay d, Position p)
        {
          this->dzn_out_p = &this->p.in.getMoveUpSlowPlanning.dzn_out_binding;
          this->p_getMoveUpSlowPlanning (current, d, p);
        };
      this->p.in.getMoveDownFastPlanning = [this] (Position current, Delay d, Position p)
        {
          this->dzn_out_p = &this->p.in.getMoveDownFastPlanning.dzn_out_binding;
          this->p_getMoveDownFastPlanning (current, d, p);
        };
      this->p.in.getMoveDownSlowPlanning = [this] (Position current, Delay d, Position p)
        {
          this->dzn_out_p = &this->p.in.getMoveDownSlowPlanning.dzn_out_binding;
          this->p_getMoveDownSlowPlanning (current, d, p);
        };
      this->p.in.setEndstopUpPosition = [this] (Position p)
        {
          this->dzn_out_p = &this->p.in.setEndstopUpPosition.dzn_out_binding;
          this->p_setEndstopUpPosition (p);
        };
      this->p.in.setEndstopDownPosition = [this] (Position p)
        {
          this->dzn_out_p = &this->p.in.setEndstopDownPosition.dzn_out_binding;
          this->p_setEndstopDownPosition (p);
        };
    }
  FPlanner::~FPlanner ()
    {}
}
// version 2.18.2
