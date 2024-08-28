#include <stdexcept>

#include "System.hh"
#include "ScreenLogger.h"
#include "MQTT_MoveUpDown.h"
#include "MQTTBridge.h"

#define ENTRY(s) logger.log(LogLevel::DEBUG, std::string("> FMoveUpDown::")+std::string(s))
#define EXIT(s) logger.log(LogLevel::DEBUG, std::string("< FMoveUpDown::")+std::string(s))
#define LOG(s) logger.log(LogLevel::DEBUG, std::string("= FMoveUpDown::")+std::string(s))

static StreamLogger logger = ScreenLogger();

FMoveUpDown::FMoveUpDown(dzn::locator const &locator)
    : skel::FMoveUpDown(locator), Foreign(), statusTopic(MQTT::MoveUpDown::getTopicStatus()), cmdTopic(MQTT::MoveUpDown::getTopicCommand())
{
    ENTRY("FMoveUpDown()");
    EXIT("FMoveUpDown()");
}

FMoveUpDown::~FMoveUpDown()
{
    ENTRY("~FMoveUpDown()");
    EXIT("~FMoveUpDown()");
}

void FMoveUpDown::connect()
{
    ENTRY("connect()");
    Foreign::connect();

    mqtt->subscribe(statusTopic, this);
    LOG("connect() - statusTopic: " + statusTopic);
    LOG("connect() - accepted body must be: " + MQTT::MoveUpDown::getMsgStatus(MQTT::MoveUpDown::Status::ABORTED));
    LOG("connect() -                    or: " + MQTT::MoveUpDown::getMsgStatus(MQTT::MoveUpDown::Status::FAILED));
    LOG("connect() -                    or: " + MQTT::MoveUpDown::getMsgStatus(MQTT::MoveUpDown::Status::FINISHED));
    LOG("connect() - commandTopic: " + cmdTopic);
    LOG("connect() -          body will be: " + MQTT::MoveUpDown::getMsgCommand(MQTT::MoveUpDown::Command::ABORT));
    LOG("connect() -                    or: " + MQTT::MoveUpDown::getMsgCommand(MQTT::MoveUpDown::Command::MOVE_DOWN));
    LOG("connect() -                    or: " + MQTT::MoveUpDown::getMsgCommand(MQTT::MoveUpDown::Command::MOVE_UP));
    EXIT("connect()");
}

void FMoveUpDown::messageReceived(std::string topic, std::string body)
{
    ENTRY("messageReceived(topic: " + topic + ", body: " + body + ")");
    if (topic == this->statusTopic)
    {
        if (body == MQTT::MoveUpDown::getMsgStatus(MQTT::MoveUpDown::ABORTED))
        {
            p_moveAborted();
        }
        if (body == MQTT::MoveUpDown::getMsgStatus(MQTT::MoveUpDown::FAILED))
        {
            p_moveFailed();
        }
        if (body == MQTT::MoveUpDown::getMsgStatus(MQTT::MoveUpDown::FINISHED))
        {
            p_moveFinished();
        }
    }
    EXIT("messageReceived(topic: " + topic + ", body: " + body + ")");
}

void FMoveUpDown::p_moveUp() { 
    mqtt->sendMsg(cmdTopic, MQTT::MoveUpDown::getMsgCommand(MQTT::MoveUpDown::Command::MOVE_UP));
}

void FMoveUpDown::p_moveDown() { 
    mqtt->sendMsg(cmdTopic, MQTT::MoveUpDown::getMsgCommand(MQTT::MoveUpDown::Command::MOVE_DOWN));
}

void FMoveUpDown::p_abort() { 
    mqtt->sendMsg(cmdTopic, MQTT::MoveUpDown::getMsgCommand(MQTT::MoveUpDown::Command::ABORT));
}
