#include <stdexcept>

#include "FButton.h"
#include "MQTT_Button.h"
#include "ScreenLogger.h"

#define ENTRY(s) logger.log(LogLevel::DEBUG, std::string("> FButton::")+std::string(s))
#define EXIT(s) logger.log(LogLevel::DEBUG, std::string("< FButton::")+std::string(s))
#define LOG(s) logger.log(LogLevel::DEBUG, std::string("= FButton::")+std::string(s))
static StreamLogger logger = ScreenLogger();

FButton::FButton()
    : ForeignWithLocation()
{
    ENTRY("FButton()");
    EXIT("FButton()");
}

FButton::~FButton()
{
    ENTRY("~FButton()");
    EXIT("~FButton()");
}

void FButton::setSystem(System *sys)
{
    ENTRY("setSystem()");
    if (sys == nullptr)
        throw new std::invalid_argument("FButton::setSystem does not accept a nullptr for system.");
    this->system = sys;
    EXIT("setSystem()");
}

void FButton::connect()
{
    ENTRY("connect()");
    ForeignWithLocation::connect();

    if (system == nullptr)
        throw new std::logic_error("FButton::connect expects system to be set.");

    topic = MQTT::Button::getTopicStatus(location);
    mqtt->subscribe(topic, this);
    LOG("connect() - topic: " + topic);
    LOG("connect() - accepted body must be: " + MQTT::Button::getMsgStatus(MQTT::Button::PRESSED));
    EXIT("connect()");
}

void FButton::messageReceived(std::string topic, std::string body)
{
    ENTRY("messageReceived(topic: " + topic + ", body: " + body + ")");
    if (topic == this->topic)
    {
        if (body == MQTT::Button::getMsgStatus(MQTT::Button::PRESSED))
        {
            switch (location)
            {
            case MQTT::Location::DOWN:
                system->p.in.btnDownPressed();
                break;
            case MQTT::Location::UP:
                system->p.in.btnUpPressed();
                break;
            default:
                throw new std::invalid_argument("FButton::msgReceived invoked while location not set.");
            }
        }
    }
    EXIT("messageReceived(topic: " + topic + ", body: " + body + ")");
}
