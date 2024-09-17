#ifndef MQTT_INTRUSIONDETECTION_H
#define MQTT_INTRUSIONDETECTION_H

#include <string>

#include "MQTT.h"

namespace MQTT
{
    namespace MoveUpDown
    {

        typedef enum
        {
            FINISHED,
            FAILED,
            ABORTED
        } Status;

        const std::string getTopicStatus();
        const std::string getMsgStatus(Status c);

        typedef enum
        {
            ABORT,
            MOVE_UP,
            MOVE_DOWN
        } Command;

        const std::string getTopicCommand();
        const std::string getMsgCommand(Command c);
    }
}

#endif
