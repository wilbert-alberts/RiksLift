#ifndef FPOSITIONSENSOR_HH
#define FPOSITIONSENSOR_HH

#include "Location.h"

class FEndstop;

class FPositionSensor : public skel::FPositionSensor
{
public:
  FPositionSensor(dzn::locator const &locator);
  virtual ~FPositionSensor();
  void setEndstopToMonitor(Location location, FEndstop* endstop);

  virtual void p_getCurrentPosition(double* posInM);
  virtual void p_getLastEndstopPosition(double* posInM);

  static void loop();

  // For FDestinationSensor
  static double getCurrentPosition();

private:
  // Functionality to capture last position the
  // endstop changed state
  bool   captureEndstop();
  FEndstop* endstopToMonitor;
  Location  location;
  double endstopPosition;
  ::IEndstop::State   previousEndstopState;

  // Functionality to retrieve the current position
  static double currentPosition;
  static double capturePosition();

  // Functionality to manage instances
  static std::vector<FPositionSensor*> instances;
};

#endif