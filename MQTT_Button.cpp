#include "MQTT_Button.h"

#include "MQTT.h"

namespace MQTT::Button
{
    static const std::string UNKNOWN("unknown");
    static const std::string STATUS_PRESSED("pressed");

    const std::string &statusToString(Status d)
    {
        switch (d)
        {
        case Status::PRESSED:
            return STATUS_PRESSED;
        default:
            return UNKNOWN;
        }
    }

    const std::string getTopicStatus(Location l) 
    {
        return ::MQTT::getTopicStatus(deviceToString(Device::BUTTON), locationToString(l));
    }

    const std::string getMsgStatus(Status c) {
        return statusToString(c);
    }
}