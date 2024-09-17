#ifndef FDESTINATIONSENSOR_HH
#define FDESTINATIONSENSOR_HH

#include <vector>


class FDestinationSensor : public skel::FDestinationSensor
{
public:
  FDestinationSensor(dzn::locator const &locator);
  virtual ~FDestinationSensor();

  virtual void p_setDestination(Position posInM);
  virtual void p_cancelDestination();

  static void loop();

private:
  void checkArrived(double currentPostion);

  static double eps;
  static std::vector<FDestinationSensor*> instances;

  bool armed;
  double destination;
};

#endif