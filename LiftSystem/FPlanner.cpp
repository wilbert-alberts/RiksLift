#include "LiftSystem.hh"

FPlanner::FPlanner(dzn::locator const &locator)
    : skel::FPlanner(locator)
{
}

FPlanner::~FPlanner()
{
}

void FPlanner::p_getMoveUpFastPlanning(Position current, Delay d, Position p)
{
  /* TODO*/
}

void FPlanner::p_getMoveUpSlowPlanning(Position current, Delay d, Position p)
{
  /* TODO*/
}

void FPlanner::p_getMoveDownFastPlanning(Position current, Delay d, Position p)
{
  /* TODO*/
}

void FPlanner::p_getMoveDownSlowPlanning(Position current, Delay d, Position p)
{
  /* TODO*/
}

void FPlanner::p_setEndstopUpPosition(Position p)
{
  /* TODO*/
}

void FPlanner::p_setEndstopDownPosition(Position p)
{
  /* TODO*/
}
