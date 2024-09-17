#ifndef FPLANNER_HH
#define FPLANNER_HH

#include "Position.hh"
#include "Delay.hh"

class FPlanner : public skel::FPlanner
{
public:
  FPlanner(dzn::locator const &locator): skel::FPlanner(locator){};
  virtual ~FPlanner(){};

  virtual void p_getMoveUpFastPlanning(Position current, Delay d, Position p){};
  virtual void p_getMoveUpSlowPlanning(Position current, Delay d, Position p){};
  virtual void p_getMoveDownFastPlanning(Position current, Delay d, Position p){};
  virtual void p_getMoveDownSlowPlanning(Position current, Delay d, Position p){};
  virtual void p_setEndstopUpPosition(Position p){};
  virtual void p_setEndstopDownPosition(Position p){};

};

#endif