#ifndef FMOVEUPDOWN_H
#define FMOVEUPDOWN_H

#include "LiftSystem.hh"

class FMoveUpDown //: public Foreign, MQTTMessageClient
{
public:
    FMoveUpDown();
    virtual ~FMoveUpDown();

    void setSystem(LiftSystem* system);
    void connect();
    void messageReceived(std::string topic, std::string body);

private:
    static void brokerlinkCB(const String &value, const size_t size);
    static FMoveUpDown* INSTANCE;

    void moveAborted();
    void moveFailed();
    void moveFinished();

private:
    LiftSystem* system;
    std::string cmdTopic;
    std::string statusTopic;


};

#endif