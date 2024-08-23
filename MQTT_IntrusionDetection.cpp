#include "MQTT_IntrusionDetection.h"

namespace MQTT::IntrusionDetection
{
    static const std::string UNKNOWN("unknown");
    static const std::string STATUS_CLEARED("cleared");
    static const std::string STATUS_TRIGGERED("triggered");
    
    const std::string &statusToString(Status d)
    {
        switch (d)
        {
        case Status::CLEARED:
            return STATUS_CLEARED;
        case Status::TRIGGERED:
            return STATUS_TRIGGERED;
        default:
            return UNKNOWN;
        }
    }

    
    const std::string getTopicStatus(Location l)
    {
        return ::MQTT::getTopicStatus(deviceToString(Device::INTRUSIONDETECTION), locationToString(l));
    }

    const std::string getTopicCommand(Location l)
    {
        return ::MQTT::getTopicCommand(deviceToString(Device::INTRUSIONDETECTION), locationToString(l));
    }

    const std::string getMsgStatus(Status c)
    {
        return statusToString(c);
    }
}