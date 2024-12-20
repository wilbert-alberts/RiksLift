#include "LiftSystem.hh"
#include "MQTTLogger.h"

#define ENTRY(s) logger.log(LogLevel::DEBUG, std::string("> FMoveUpDown::") + std::string(s))
#define EXIT(s) logger.log(LogLevel::DEBUG, std::string("< FMoveUpDown::") + std::string(s))
#define LOG(s) logger.log(LogLevel::DEBUG, std::string("= FMoveUpDown::") + std::string(s))
static MQTTLogger logger = MQTTLogger();

#define _DEBUG 1
#include "debug.h"

// static members FPlanner
bool FPlanner::endstopUpValid = false;
double FPlanner::endstopUp = 0.0;
bool FPlanner::endstopDownValid = false;
double FPlanner::endstopDown = 0.0;

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
    .velocityFast = 0.1,
    .velocitySlow = 0.01,
    .positionOffsetToGoSlow = 0.2,
    .positionOffsetToStop = -0.02, // 5 cm higher then endstop
    .moveTimeExceededTimeout = 10};

static PlannerParameters downParameters = {
    .velocityFast = 0.1,
    .velocitySlow = 0.01,
    .positionOffsetToGoSlow = 0.2,
    .positionOffsetToStop = 0.02, // 5cm lower then endstop
    .moveTimeExceededTimeout = 10};

FPlanner::FPlanner(dzn::locator const &locator)
    : skel::FPlanner(locator) //, endstopUpValid(false), endstopUp(0.0), endstopDownValid(false), endstopDown(0.0)
{
}

FPlanner::~FPlanner()
{
}

void FPlanner::p_getMoveUpFastPlanning(double current, double* delay, double* position)
{
  ENTRY(std::string("FPlanner::p_getMoveUpFastPlanning()"));

  std::string currentStr = std::to_string(current);
  LOG(std::string("FPlanner::p_getMoveUpFastPlanning(), current: ") + currentStr);

  if (!endstopUpValid)
  {
    LOG("*** Exc Cannot make planning for fast up movement if endstop up position has not been set");
    throw std::invalid_argument("Cannot make planning for fast up movement if endstop up position has not been set.");
  }

  double targetPosition = endstopUp + upParameters.positionOffsetToGoSlow;
  LOG(std::string("FPlanner::p_getMoveUpFastPlanning(), targetPosition: ") + std::to_string(targetPosition));

  double distance = abs(targetPosition - current);
  LOG(std::string("FPlanner::p_getMoveUpFastPlanning(), distance: ") + std::to_string(distance));

  double nominalTime = distance / upParameters.velocityFast;
  LOG(std::string("FPlanner::p_getMoveUpFastPlanning(), nominalTime: ") + std::to_string(nominalTime));

  double totalTime = nominalTime + upParameters.moveTimeExceededTimeout;
  LOG(std::string("FPlanner::p_getMoveUpFastPlanning(), totalTime: ") + std::to_string(totalTime));

  *delay = totalTime;
  *position = targetPosition;
  EXIT("FPlanner::p_getMoveUpFastPlanning");
}

void FPlanner::p_getMoveUpSlowPlanning(double current, double* delay, double* position)
{
  ENTRY("FPlanner::p_getMoveUpSlowPlanning");
  if (!endstopUpValid)
  {
    LOG("*** Exc Cannot make planning for slow up movement if endstop up position has not been set");
    throw std::invalid_argument("Cannot make planning for slow up movement if endstop up position has not been set.");
  }

  double targetPositon = endstopUp + upParameters.positionOffsetToStop;
  double distance = abs(targetPositon - current);
  double nominalTime = distance / upParameters.velocitySlow;
  double totalTime = nominalTime + upParameters.moveTimeExceededTimeout;

  *delay = totalTime;
  *position = targetPositon;
  EXIT("FPlanner::p_getMoveUpSlowPlanning");
}

void FPlanner::p_getMoveDownFastPlanning(double current, double* delay, double* position)
{
  ENTRY("FPlanner::p_getMoveUpFastPlanning");
  if (!endstopDownValid)
  {
    LOG("*** Exc Cannot make planning for fast down movement if endstop up position has not been set");
    throw std::invalid_argument("Cannot make planning for fast down movement if endstop down position has not been set.");
  }

  double targetPositon = endstopDown + downParameters.positionOffsetToGoSlow;
  double distance = abs(targetPositon - current);
  double nominalTime = distance / downParameters.velocityFast;
  double totalTime = nominalTime + downParameters.moveTimeExceededTimeout;

  *delay = totalTime;
  *position = targetPositon;
  EXIT("FPlanner::p_getMoveUpFastPlanning");
}

void FPlanner::p_getMoveDownSlowPlanning(double current, double* delay, double* position)
{
  ENTRY("FPlanner::p_getMoveDownSlowPlanning");
  if (!endstopDownValid)
  {
    LOG("*** Exc Cannot make planning for slow down movement if endstop up position has not been set");
    throw std::invalid_argument("Cannot make planning for slow down movement if endstop down position has not been set.");
  }

  double targetPositon = endstopDown + downParameters.positionOffsetToGoSlow;
  double distance = abs(targetPositon - current);
  double nominalTime = distance / downParameters.velocitySlow;
  double totalTime = nominalTime + downParameters.moveTimeExceededTimeout;

  *delay = totalTime;
  *position = targetPositon;
  EXIT("FPlanner::p_getMoveDownSlowPlanning");
}

void FPlanner::p_setEndstopUpPosition(double position)
{
  DEBUG((">  FPlanner::p_setEndstopUpPosition(position: " + std::to_string(position) + ")\n").c_str());
  endstopUp = position;
  endstopUpValid = true;
  DEBUG("<  FPlanner::p_setEndstopUpPosition()\n");
}

void FPlanner::p_setEndstopDownPosition(double position)
{
  DEBUG((">  FPlanner::p_setEndstopDownPosition(position: " + std::to_string(position) + ")\n").c_str());
  endstopDown = position;
  endstopDownValid = true;
  DEBUG("<  FPlanner::p_setEndstopDownPosition()\n");
}
