#ifndef MQTT_INTRUSIONDETECTION_H
#define MQTT_INTRUSIONDETECTION_H

#include <string>

#include "MQTT.h"

namespace MQTT
{
    namespace IntrusionDetection
    {

        typedef enum
        {
            TRIGGERED,
            CLEARED
        } Status;

        const std::string getTopicStatus(Location l);
        const std::string getMsgStatus(Status c);
    }
}

#endif
