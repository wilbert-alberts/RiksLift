#include "MQTT_MoveUpDown.h"

namespace MQTT::MoveUpDown
{
    static const std::string UNKNOWN("unknown");
    static const std::string STATUS_FINISHED("moveFinished");
    static const std::string STATUS_FAILED("moveFailed");
    static const std::string STATUS_ABORTED("moveAborted");
    static const std::string CMD_MOVE_DOWN("moveDown");
    static const std::string CMD_MOVE_UP("moveUp");
    static const std::string CMD_ABORT("abortMove");
    
    
    const std::string getTopicStatus()
    {
        return ::MQTT::getTopicStatus(deviceToString(Device::MOVEUPDOWN));
    }

    const std::string getTopicCommand()
    {
        return ::MQTT::getTopicCommand(deviceToString(Device::MOVEUPDOWN));
    }

    const std::string &statusToString(Status d)
    {
        switch (d)
        {
        case Status::ABORTED:
            return STATUS_ABORTED;
        case Status::FAILED:
            return STATUS_FAILED;
        case Status::FINISHED:
            return STATUS_FINISHED;
        default:
            return UNKNOWN;
        }
    }
    const std::string getMsgStatus(Status c)
    {
        return statusToString(c);
    }
    

    const std::string &commandToString(Command d)
    {
        switch (d)
        {
        case Command::ABORT:
            return CMD_ABORT;
        case Command::MOVE_DOWN:
            return CMD_MOVE_DOWN;
        case Command::MOVE_UP:
            return CMD_MOVE_UP;
        default:
            return UNKNOWN;
        }
    }
    const std::string getMsgCommand(Command c)
    {
        return commandToString(c);
    }
    


}