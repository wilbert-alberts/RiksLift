#ifndef MQTTLOGGER_H
#define MQTTLOGGER_H

#include "BaseLogger.h"
#include "brokerlink.h"

class MQTTLogger : public BaseLogger
{
public:
    MQTTLogger(): topic("Liftsystem/debug") {};
    MQTTLogger(const std::string& tpc): topic(String(tpc.c_str())) {};
    virtual ~MQTTLogger(){}

    virtual void logMessage(const std::string& msg) {
        String arduinoMsg = String(msg.c_str());
        brokerLink.publish(topic, arduinoMsg, false);
    }
private:
    String topic;
};

#endif