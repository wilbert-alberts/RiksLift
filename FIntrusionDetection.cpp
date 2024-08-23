#include <stdexcept>

#include "SafetySystem.hh"
#include "ScreenLogger.h"
#include "MQTTBridge.h"
#include "MQTT_IntrusionDetection.h"

#define ENTRY(s) logger.log(LogLevel::DEBUG, std::string("> FIntrusionDetection::")+std::string(s))
#define EXIT(s) logger.log(LogLevel::DEBUG, std::string("< FIntrusionDetection::")+std::string(s))
#define LOG(s) logger.log(LogLevel::DEBUG, std::string("= FIntrusionDetection::")+std::string(s))

static StreamLogger logger = ScreenLogger();

FIntrusionDetection::FIntrusionDetection(dzn::locator const &locator)
    : skel::FIntrusionDetection(locator), ForeignWithLocation(), topic("")
{
    ENTRY("FIntrusionDetection()");
    EXIT("FIntrusionDetection()");
}

FIntrusionDetection::~FIntrusionDetection()
{
    ENTRY("~FIntrusionDetection()");
    EXIT("~FIntrusionDetection()");
}

void FIntrusionDetection::connect()
{
    ENTRY("connect()");
    Foreign::connect();

    topic = MQTT::IntrusionDetection::getTopicStatus(location);
    mqtt->subscribe(topic, this);
    LOG("connect() - topic: " + topic);
    LOG("connect() - accepted body must be: " + MQTT::IntrusionDetection::getMsgStatus(MQTT::IntrusionDetection::TRIGGERED));
    LOG("connect() -                    or: " + MQTT::IntrusionDetection::getMsgStatus(MQTT::IntrusionDetection::CLEARED));
    EXIT("connect()");
}

void FIntrusionDetection::messageReceived(std::string topic, std::string body)
{
    ENTRY("messageReceived(topic: " + topic + ", body: " + body + ")");
    if (topic == this->topic)
    {
        if (body == MQTT::IntrusionDetection::getMsgStatus(MQTT::IntrusionDetection::TRIGGERED))
        {
            p_triggered();
        }
        if (body == MQTT::IntrusionDetection::getMsgStatus(MQTT::IntrusionDetection::CLEARED))
        {
            p_cleared();
        }
    }
    EXIT("messageReceived(topic: " + topic + ", body: " + body + ")");
}
