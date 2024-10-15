#include <Arduino.h>
#include <stdexcept>

#include "FMoveUpDown.h"
#include "MQTT_MoveUpDown.h"
// #include "ScreenLogger.h"
#include "MQTTLogger.h"

#include "brokerlink.h"

#define ENTRY(s) logger.log(LogLevel::DEBUG, std::string("> FMoveUpDown::") + std::string(s))
#define EXIT(s) logger.log(LogLevel::DEBUG, std::string("< FMoveUpDown::") + std::string(s))
#define LOG(s) logger.log(LogLevel::DEBUG, std::string("= FMoveUpDown::") + std::string(s))
static MQTTLogger logger = MQTTLogger();

void FMoveUpDown::blPublish(std::string body)
{
    String topicString = String(statusTopic.c_str());
    brokerLink.publish(topicString, String(body.c_str()), false);
}

FMoveUpDown::FMoveUpDown()
    : cmdTopic(MQTT::MoveUpDown::getTopicCommand()), statusTopic(MQTT::MoveUpDown::getTopicStatus())
{
    ENTRY("FMoveUpDown ()");
    if (INSTANCE == nullptr)
    {
        INSTANCE = this;
    }
    else
    {
        throw new std::logic_error("Should not create more than one FMoveUpDown");
    }
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

    if (system == nullptr)
        throw new std::logic_error("FMoveUpDown::connect expects system to be set.");

    system->mudArmor.p.out.moveAborted = [this]
    { this->moveAborted(); };
    system->mudArmor.p.out.moveFailed = [this]
    { this->moveFailed(); };
    system->mudArmor.p.out.moveFinished = [this]
    { this->moveFinished(); };


    String topicStr = cmdTopic.c_str();
    brokerLink.subscribe(topicStr, FMoveUpDown::brokerlinkCB, false);

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

FMoveUpDown *FMoveUpDown::INSTANCE;
void FMoveUpDown::brokerlinkCB(const String &value, const size_t size)
{
    INSTANCE->messageReceived(INSTANCE->cmdTopic, value.c_str());
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

void FMoveUpDown::moveFinished()
{
    // send mqtt message with:
    //   topic: statusTopic
    //   body:  MQTT::MoveUpDown::statusToString(MQTT::MoveUpDown::Status::STATUS_FINISHED)
    ENTRY("FMoveUpDown::moveFinished");
    blPublish(MQTT::MoveUpDown::statusToString(MQTT::MoveUpDown::Status::FINISHED));
    EXIT("FMoveUpDown::moveFinished");
}

void FMoveUpDown::moveFailed()
{
    // send mqtt message with:
    //   topic: statusTopic
    //   body:  MQTT::MoveUpDown::statusToString(MQTT::MoveUpDown::Status::STATUS_FAILED)
    ENTRY("FMoveUpDown::moveFailed");
    blPublish(MQTT::MoveUpDown::statusToString(MQTT::MoveUpDown::Status::FAILED));
    EXIT("FMoveUpDown::moveFailed");
}

void FMoveUpDown::moveAborted()
{
    // send mqtt message with:
    //   topic: statusTopic
    //   body:  MQTT::MoveUpDown::statusToString(MQTT::MoveUpDown::Status::STATUS_ABORTED)
    ENTRY("FMoveUpDown::moveAborted");
    blPublish(MQTT::MoveUpDown::statusToString(MQTT::MoveUpDown::Status::ABORTED));
    EXIT("FMoveUpDown::moveAborted");
}
