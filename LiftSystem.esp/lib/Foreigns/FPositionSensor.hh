#ifndef FPOSITIONSENSOR_HH
#define FPOSITIONSENSOR_HH

class FEndstop;

class FPositionSensor : public skel::FPositionSensor
{
public:
  FPositionSensor(dzn::locator const &locator);
  virtual ~FPositionSensor();
  void setEndstopToMonitor(FEndstop* endstop);

  virtual void p_getCurrentPosition(Position posInM);
  virtual void p_getLastEndstopPosition(Position posInM);

  static void loop();

  // For FDestinationSensor
  static double getCurrentPosition();

private:
  // Functionality to capture last position the
  // endstop changed state
  bool   captureEndstop();
  FEndstop* endstopToMonitor;
  double endstopPosition;
  ::IEndstop::State   previousEndstopState;

  // Functionality to retrieve the current position
  static double currentPosition;
  static double capturePosition();

  // Functionality to manage instances
  static std::vector<FPositionSensor*> instances;
};

#endif