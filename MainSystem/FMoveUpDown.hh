#ifndef FMOVEUPDOWN
#define FMOVEUPDOWN

#include "MQTTBridge.h"
#include "Foreign.h"

struct FMoveUpDown : public skel::FMoveUpDown, public MQTTMessageClient, public Foreign
{
public:
  FMoveUpDown(const dzn::locator &locator);
  virtual ~FMoveUpDown();

  void connect();
  void messageReceived(std::string topic, std::string body);

  virtual void p_moveUp();
  virtual void p_moveDown();
  virtual void p_abort();

private:
  std::string statusTopic;
  std::string cmdTopic;
};

#endif