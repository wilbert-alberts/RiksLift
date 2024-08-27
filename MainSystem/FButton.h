#ifndef FBUTTON_H
#define FBUTTON_H

#include <string>

#include "System.hh"
#include "MQTTBridge.h"
#include "ForeignWithLocation.h"


class FButton: public MQTTMessageClient, public ForeignWithLocation
{
public:
    FButton();
    virtual ~FButton();

    void setSystem(System* sys);
    void connect();
    void messageReceived(std::string topic, std::string body);

private:
    System* system;
    std::string topic;
};

#endif