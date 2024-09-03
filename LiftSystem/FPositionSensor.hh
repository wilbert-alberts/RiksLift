#ifndef FPOSITIONSENSOR_HH
#define FPOSITIONSENSOR_HH

class FPositionSensor : public skel::FPositionSensor
{
public:
  FPositionSensor(dzn::locator const &locator);
  virtual ~FPositionSensor();

  virtual void p_getCurrentPosition(Position posInM);
  virtual void p_getLastEndstopPosition(Position posInM);

  static void loop();

  // For FDestinationSensor
  static double getCurrentPosition();

private:
  static double currentPosition;
  static double endstopPosition;
  static bool   previousEndstopState;

  static double capturePosition();
  static bool   captureEndstop();

};

#endif