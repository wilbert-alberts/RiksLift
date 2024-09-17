#include "Foreign.h"

#include <stdexcept>

#define _DEBUG 1
#include "debug.h"

#define ENTRY(s) DEBUG((std::string("> Foreign::")+std::string(s)).c_str())
#define EXIT(s) DEBUG((std::string("< Foreign::")+std::string(s)).c_str())
#define LOG(s) DEBUG((std::string("= Foreign::")+std::string(s)).c_str())


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