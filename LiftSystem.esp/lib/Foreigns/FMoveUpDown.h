#ifndef FMOVEUPDOWN_H
#define FMOVEUPDOWN_H

#include "Foreign.h"
#include "LiftSystem.hh"
// #include "MQTTBridge.h"

class FMoveUpDown : public Foreign //, MQTTMessageClient
{
public:
    FMoveUpDown();
    virtual ~FMoveUpDown();

    void setSystem(LiftSystem* system);
    void connect();
    void messageReceived(std::string topic, std::string body);

private:
    void moveAborted();
    void moveFailed();
    void moveFinished();

private:
    LiftSystem* system;
    std::string cmdTopic;
    std::string statusTopic;


};

#endif