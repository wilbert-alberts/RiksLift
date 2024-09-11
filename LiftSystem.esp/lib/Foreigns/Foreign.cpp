#include "Foreign.h"

#include <stdexcept>
#include "ScreenLogger.h"

#define ENTRY(s) logger.log(LogLevel::DEBUG, std::string("> Foreign::")+std::string(s))
#define EXIT(s) logger.log(LogLevel::DEBUG, std::string("< Foreign::")+std::string(s))
#define LOG(s) logger.log(LogLevel::DEBUG, std::string("= Foreign::")+std::string(s))

static StreamLogger logger = ScreenLogger();

Foreign::Foreign()
    : mqtt(nullptr)
{
    ENTRY("Foreign()");
    EXIT("Foreign()");
}

Foreign::~Foreign()
{
    ENTRY("~Foreign()");
    EXIT("~Foreign()");
}

void Foreign::setMQTTBridge(MQTTBridge *mqtt)
{
    ENTRY("setMQTTBridge()");
    if (mqtt == nullptr)
        throw new std::invalid_argument("Foreign::setMQTTBridge does not accept nullptr for mqtt. ");
    this->mqtt = mqtt;
    EXIT("setMQTTBridge()");
}

void Foreign::connect()
{
    ENTRY("connect()");
    if (mqtt == nullptr)
        throw new std::logic_error("Foreign::connect expects mqtt to be set but it isn't. ");
    EXIT("connect()");
}