#ifndef FPLANNER_HH
#define FPLANNER_HH

class FPlanner : public skel::FPlanner
{
public:
    FPlanner(dzn::locator const& locator): skel::FPlanner(locator) {};
    virtual ~FPlanner() {};

      virtual void p_getMoveUpFastDelay (Delay d)  { /* TODO*/ };
      virtual void p_getMoveUpFastPosition (Position p)  { /* TODO*/ };
      virtual void p_getMoveDownFastDelay (Delay d) { /* TODO*/ };
      virtual void p_getMoveDownFastPosition (Position p) { /* TODO*/ };
      virtual void p_setEndstopUpPosition (Position p) { /* TODO*/ };
      virtual void p_setEndstopDownPosition (Position p)  { /* TODO*/ };
};

#endif