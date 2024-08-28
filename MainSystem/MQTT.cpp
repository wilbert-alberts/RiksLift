#include "MQTT.h"

namespace MQTT
{
    static const std::string UNKNOWN("unknown");

    static const std::string DEVICE_INTRUSIONDETECTION("intrusiondetection");
    static const std::string DEVICE_MOVEUPDOWN("lift");
    static const std::string DEVICE_BUTTON("button");

    static const std::string LOCATION_UP("up");
    static const std::string LOCATION_DOWN("down");

    static const std::string MSG_KIND_COMMAND("command");
    static const std::string MSG_KIND_STATUS("status");

    const std::string &deviceToString(Device d)
    {
        switch (d)
        {
        case Device::INTRUSIONDETECTION:
            return DEVICE_INTRUSIONDETECTION;
        case Device::MOVEUPDOWN:
            return DEVICE_MOVEUPDOWN;
        case Device::BUTTON:
            return DEVICE_BUTTON;
        default:
            return UNKNOWN;
        }
    }

    const std::string &locationToString(Location d)
    {
        switch (d)
        {
        case Location::UP:
            return LOCATION_UP;
        case Location::DOWN:
            return LOCATION_DOWN;
        default:
            return UNKNOWN;
        }
    }

    const std::string &messageKindToString(MessageKind d)
    {
        switch (d)
        {
        case MessageKind::STATUS:
            return MSG_KIND_STATUS;
        case MessageKind::COMMAND:
            return MSG_KIND_COMMAND;
        default:
            return UNKNOWN;
        }
    }

    const std::string getTopicStatus(std::string device, std::string location)
    {
        return device + "/" + location + "/" + messageKindToString(MessageKind::STATUS);
    }
    const std::string getTopicStatus(std::string device)
    {
        return device + "/" + messageKindToString(MessageKind::STATUS);
    }
    const std::string getTopicCommand(std::string device, std::string location)
    {
        return device + "/" + location + "/" + messageKindToString(MessageKind::COMMAND);
    }
    const std::string getTopicCommand(std::string device)
    {
        return device + "/" + messageKindToString(MessageKind::COMMAND);
    }
}
