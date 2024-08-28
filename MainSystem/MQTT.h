#ifndef MQTT_H
#define MQTT_H

#include <string>

namespace MQTT
{
    typedef enum
    {
        INTRUSIONDETECTION,
        MOVEUPDOWN,
        BUTTON
    } Device;

    const std::string &deviceToString(Device d);

    typedef enum
    {
        NONE,
        UP,
        DOWN
    } Location;

    const std::string &locationToString(Location d);

    typedef enum
    {
        STATUS,
        COMMAND
    } MessageKind;

    const std::string &messageKindToString(MessageKind d);

    const std::string getTopicStatus(std::string device, std::string location);
    const std::string getTopicStatus(std::string device);
    const std::string getTopicCommand(std::string device, std::string location);
    const std::string getTopicCommand(std::string device);
}

#endif