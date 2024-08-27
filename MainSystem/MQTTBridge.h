#ifndef MQTTBRIDGE_H
#define MQTTBRIDGE_H

#include <string>
#include <set>
#include <map>
#include <mqtt/client.h>

class MQTTMessageClient
{
public:
    virtual void messageReceived(std::string topic, std::string body) = 0;
};

class MQTTBridge
{
public:
    MQTTBridge();
    virtual ~MQTTBridge();

    void setServer(const std::string &uri);

    void processMessages();

    void sendMsg(const std::string &topic, const std::string &body);
    void subscribe(const std::string &topic, MQTTMessageClient *client);

private:
    void connectToBroker();
    void initializeConnectionOptions();
    void performSubscriptions();
    void consumeMessages();
    void distributeMessage(const std::string &topic, const std::string &body);

    std::map<std::string, std::set<MQTTMessageClient *>> subscriptions;
    std::string serverURI;

    mqtt::client::ptr_t client;
    mqtt::connect_options::ptr_t connectionOptions;
};

#endif