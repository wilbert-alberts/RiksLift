#ifndef FINTRUSION_DETECTION_H
#define FINTRUSION_DETECTION_H

#include <string>

#include "MQTTBridge.h"
#include "ForeignWithLocation.h"


class FIntrusionDetection: public skel::FIntrusionDetection, public MQTTMessageClient, public ForeignWithLocation
{
public:
    FIntrusionDetection(dzn::locator const& locator);
    virtual ~FIntrusionDetection();

    void connect();
    void messageReceived(std::string topic, std::string body);

private:
    std::string topic;
};

#endif