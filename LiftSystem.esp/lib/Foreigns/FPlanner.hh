#ifndef FPLANNER_HH
#define FPLANNER_HH

class FPlanner : public skel::FPlanner
{
public:
  FPlanner(dzn::locator const &locator);
  virtual ~FPlanner();

  virtual void p_getMoveUpFastPlanning(double current, double* delay, double* position);
  virtual void p_getMoveUpSlowPlanning(double current, double* delay, double* position);
  virtual void p_getMoveDownFastPlanning(double current, double* delay, double* position);
  virtual void p_getMoveDownSlowPlanning(double current, double* delay, double* position);
  virtual void p_setEndstopUpPosition(double p);
  virtual void p_setEndstopDownPosition(double p);

private:
  static bool endstopUpValid;
  static double endstopUp;
  static bool endstopDownValid;
  static double endstopDown;
};

#endif