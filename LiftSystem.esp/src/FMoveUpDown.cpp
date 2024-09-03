#include <stdexcept>

#include "FMoveUpDown.h"
#include "MQTT_MoveUpDown.h"
#include "ScreenLogger.h"

#define ENTRY(s) logger.log(LogLevel::DEBUG, std::string("> FMoveUpDown::") + std::string(s))
#define EXIT(s) logger.log(LogLevel::DEBUG, std::string("< FMoveUpDown::") + std::string(s))
#define LOG(s) logger.log(LogLevel::DEBUG, std::string("= FMoveUpDown::") + std::string(s))
static StreamLogger logger = ScreenLogger();

FMoveUpDown::FMoveUpDown()
    : Foreign(), cmdTopic(MQTT::MoveUpDown::getTopicCommand()), statusTopic(MQTT::MoveUpDown::getTopicStatus())
{
    ENTRY("FMoveUpDown ()");
    EXIT("FMoveUpDown ()");
}

FMoveUpDown ::~FMoveUpDown()
{
    ENTRY("~FMoveUpDown ()");
    EXIT("~FMoveUpDown ()");
}

void FMoveUpDown ::setSystem(LiftSystem *sys)
{
    ENTRY("setSystem()");
    if (sys == nullptr)
        throw new std::invalid_argument("FMoveUpDown ::setSystem does not accept a nullptr for system.");
    this->system = sys;
    EXIT("setSystem()");
}

void FMoveUpDown::connect()
{
    ENTRY("connect()");
    Foreign::connect();

    if (system == nullptr)
        throw new std::logic_error("FMoveUpDown::connect expects system to be set.");

    // mqtt->subscribe(cmdTopic, this);
    LOG("connect() - cmdTopic: " + cmdTopic);
    LOG("connect() - cmdTopic body must be: " + MQTT::MoveUpDown::getMsgCommand(MQTT::MoveUpDown::Command::MOVE_UP));
    LOG("connect() -                    be: " + MQTT::MoveUpDown::getMsgCommand(MQTT::MoveUpDown::Command::MOVE_DOWN));
    LOG("connect() -                    be: " + MQTT::MoveUpDown::getMsgCommand(MQTT::MoveUpDown::Command::ABORT));
    LOG("connect() - statusTopic: " + statusTopic);
    LOG("connect() -          body will be: " + MQTT::MoveUpDown::getMsgStatus(MQTT::MoveUpDown::Status::ABORTED));
    LOG("connect() -                    be: " + MQTT::MoveUpDown::getMsgStatus(MQTT::MoveUpDown::Status::FINISHED));
    LOG("connect() -                    be: " + MQTT::MoveUpDown::getMsgStatus(MQTT::MoveUpDown::Status::FAILED));
    EXIT("connect()");
}

void FMoveUpDown::messageReceived(std::string topic, std::string body)
{
    ENTRY("messageReceived(topic: " + topic + ", body: " + body + ")");
    if (topic == this->cmdTopic)
    {
        if (body == MQTT::MoveUpDown::getMsgCommand(MQTT::MoveUpDown::Command::ABORT))
        {
            system->mudArmor.p.in.abort();
        }
        if (body == MQTT::MoveUpDown::getMsgCommand(MQTT::MoveUpDown::Command::MOVE_UP))
        {
            system->mudArmor.p.in.moveUp();
        }
        if (body == MQTT::MoveUpDown::getMsgCommand(MQTT::MoveUpDown::Command::MOVE_DOWN))
        {
            system->mudArmor.p.in.moveDown();
        }
    }
    EXIT("messageReceived(topic: " + topic + ", body: " + body + ")");
}
