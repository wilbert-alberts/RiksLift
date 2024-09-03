#ifndef FOREIGN_H
#define FOREIGN_H

// #include "MQTTBridge.h"

class MQTTBridge;

class Foreign
{
public:
    Foreign();
    virtual ~Foreign();

    void setMQTTBridge(MQTTBridge* mqtt);
    virtual void connect();

protected: 
    MQTTBridge* mqtt;
};

#endif