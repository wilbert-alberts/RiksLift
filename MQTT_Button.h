#ifndef MQTT_BUTTON_H
#define MQTT_BUTTON_H

#include <string>

#include "MQTT.h"

namespace MQTT::Button
{
    typedef enum
    {
        PRESSED,
    } Status;

    const std::string getTopicStatus(Location l);
    const std::string getMsgStatus(Status c);
}

#endif