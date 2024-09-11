#include "LiftSystem.hh"

typedef struct
{
  // Velocities to move fast/slow in m/s
  double velocityFast;
  double velocitySlow;

  // Position offsets w.r.t. endstop in order to
  // slow down resp. stop
  double positionOffsetToGoSlow;
  double positionOffsetToStop;

  // How many seconds to add to nominal movetime
  // in order to consider move to have failed.
  double moveTimeExceededTimeout;

} PlannerParameters;

static PlannerParameters upParameters = {
    .velocityFast = 1.0,
    .velocitySlow = 0.01,
    .positionOffsetToGoSlow = 0,
    .positionOffsetToStop = -0.05, // 5 cm higher then endstop
    .moveTimeExceededTimeout = 10};

static PlannerParameters downParameters = {
    .velocityFast = 1.0,
    .velocitySlow = 0.01,
    .positionOffsetToGoSlow = 0,
    .positionOffsetToStop = 0.05, // 5cm lower then endstop
    .moveTimeExceededTimeout = 10};

FPlanner::FPlanner(dzn::locator const &locator)
    : skel::FPlanner(locator), endstopUpValid(false), endstopUp(0.0), endstopDownValid(false), endstopDown(0.0)
{
}

FPlanner::~FPlanner()
{
}

void FPlanner::p_getMoveUpFastPlanning(Position current, Delay d, Position p)
{
  if (!endstopUpValid)
  {
    throw std::invalid_argument("Cannot make planning for up movement if endstop up position has not been set.");
  }

  double targetPositon = endstopUp + upParameters.positionOffsetToGoSlow;
  double distance = abs(targetPositon - current.getPosition());
  double nominalTime = distance / upParameters.velocityFast;
  double totalTime = nominalTime + upParameters.moveTimeExceededTimeout;

  d.setDelay(totalTime);
  p.setPosition(targetPositon);
}

void FPlanner::p_getMoveUpSlowPlanning(Position current, Delay d, Position p)
{
  if (!endstopUpValid)
  {
    throw std::invalid_argument("Cannot make planning for up movement if endstop up position has not been set.");
  }

  double targetPositon = endstopUp + upParameters.positionOffsetToStop;
  double distance = abs(targetPositon - current.getPosition());
  double nominalTime = distance / upParameters.velocitySlow;
  double totalTime = nominalTime + upParameters.moveTimeExceededTimeout;

  d.setDelay(totalTime);
  p.setPosition(targetPositon);
}

void FPlanner::p_getMoveDownFastPlanning(Position current, Delay d, Position p)
{
  if (!endstopDownValid)
  {
    throw std::invalid_argument("Cannot make planning for up movement if endstop down position has not been set.");
  }

  double targetPositon = endstopDown + downParameters.positionOffsetToGoSlow;
  double distance = abs(targetPositon - current.getPosition());
  double nominalTime = distance / downParameters.velocityFast;
  double totalTime = nominalTime + downParameters.moveTimeExceededTimeout;

  d.setDelay(totalTime);
  p.setPosition(targetPositon);
}

void FPlanner::p_getMoveDownSlowPlanning(Position current, Delay d, Position p)
{
  if (!endstopDownValid)
  {
    throw std::invalid_argument("Cannot make planning for up movement if endstop down position has not been set.");
  }

  double targetPositon = endstopDown + downParameters.positionOffsetToGoSlow;
  double distance = abs(targetPositon - current.getPosition());
  double nominalTime = distance / downParameters.velocitySlow;
  double totalTime = nominalTime + downParameters.moveTimeExceededTimeout;

  d.setDelay(totalTime);
  p.setPosition(targetPositon);
}

void FPlanner::p_setEndstopUpPosition(Position p)
{
  endstopUp = p.getPosition();
  endstopUpValid = true;
}

void FPlanner::p_setEndstopDownPosition(Position p)
{
  endstopDown = p.getPosition();
  endstopDownValid = true;
}
