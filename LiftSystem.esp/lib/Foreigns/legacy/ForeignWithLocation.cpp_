#include "ForeignWithLocation.h"

#include <stdexcept>
#include "ScreenLogger.h"

#define ENTRY(s) logger.log(LogLevel::DEBUG, std::string("> ForeignWithLocation::")+std::string(s))
#define EXIT(s) logger.log(LogLevel::DEBUG, std::string("< ForeignWithLocation::")+std::string(s))
#define LOG(s) logger.log(LogLevel::DEBUG, std::string("= ForeignWithLocation::")+std::string(s))

static StreamLogger logger = ScreenLogger();

ForeignWithLocation::ForeignWithLocation()
    : Foreign(),
      location(MQTT::Location::NONE)
{
    ENTRY("ForeignWithLocation()");
    EXIT("ForeignWithLocation()");
}

ForeignWithLocation::~ForeignWithLocation()
{
    ENTRY("~ForeignWithLocation()");
    EXIT("~ForeignWithLocation()");
}

void ForeignWithLocation::setLocation(MQTT::Location location)
{
    ENTRY("setLocation(location: " + MQTT::locationToString(location) + ")");
    if (location == MQTT::Location::NONE)
        throw new std::invalid_argument("ForeignWithLocation::setLocation does not accept NONE as location.");
    this->location = location;
    EXIT("setLocation(location: " + MQTT::locationToString(location) + ")");
}

void ForeignWithLocation::connect()
{
    ENTRY("connect()");
    Foreign::connect();

    if (location == MQTT::Location::NONE)
        throw new std::logic_error("ForeignWithLocation::connect expects floor to be set unequal to UNKNOWN.");
    EXIT("connect()");
}