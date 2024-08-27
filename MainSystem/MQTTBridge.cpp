
#include <memory>

#include "ScreenLogger.h"
#include "MQTTBridge.h"

#include <mqtt/buffer_ref.h>
#include <mqtt/message.h>

#define ENTRY(s) logger.log(LogLevel::DEBUG, std::string("> MQTTBridge::") + std::string(s))
#define EXIT(s) logger.log(LogLevel::DEBUG, std::string("< MQTTBridge::") + std::string(s))
#define LOG(s) logger.log(LogLevel::DEBUG, std::string("= MQTTBridge::") + std::string(s))

static StreamLogger logger = ScreenLogger();

MQTTBridge::MQTTBridge()
    : subscriptions(), serverURI("") //, client(nullptr), connectionOptions()
{
    ENTRY("MQTTBridge()");
    EXIT("MQTTBridge()");
}

MQTTBridge::~MQTTBridge()
{
    ENTRY("~MQTTBridge()");
    EXIT("~MQTTBridge()");
};

void MQTTBridge::setServer(const std::string &uri)
{
    ENTRY("setServer(uri: " + uri + ")");
    serverURI = uri;
    EXIT("setServer(uri: " + uri + ")");
}

void MQTTBridge::processMessages()
{
    ENTRY("processMessages()");
    connectToBroker();
    performSubscriptions();
    consumeMessages();
    EXIT("processMessages()");
}

void MQTTBridge::connectToBroker()
{
    ENTRY("connectToBroker()");
    client = std::make_shared<mqtt::client>(serverURI, std::string("RicksLiftApplication"), nullptr);
    initializeConnectionOptions();
    client->connect(*connectionOptions);
    EXIT("connectToBroker()");
}

void MQTTBridge::initializeConnectionOptions()
{
    ENTRY("initializeConnectionOptions()");
    if (connectionOptions == nullptr)
    {
        connectionOptions = std::make_shared<mqtt::connect_options>();
        connectionOptions->set_automatic_reconnect(1, 300);
        connectionOptions->set_connect_timeout(60);
        connectionOptions->set_automatic_reconnect(true);
    }
    EXIT("initializeConnectionOptions()");
}

void MQTTBridge::performSubscriptions()
{
    ENTRY("performSubscriptions()");
    for (auto iter = subscriptions.begin(); iter != subscriptions.end(); iter++)
    {
        auto entry = *iter;
        std::string topic = entry.first;
        LOG("performSubscriptions() - subscribing on: " + topic);
        client->subscribe(topic);
    }
    EXIT("performSubscriptions()");
}

void MQTTBridge::consumeMessages()
{
    ENTRY("consumeMessages()");
    client->start_consuming();
    while (client->is_connected())
    {
        auto msg = client->consume_message();
        LOG("consumeMessages() - received: topic: " + msg->get_topic() + ", body: " + msg->get_payload());
        distributeMessage(msg->get_topic(), msg->get_payload_str());
    }
    client->stop_consuming();
    EXIT("consumeMessages()");
}

void MQTTBridge::distributeMessage(const std::string &topic, const std::string &body)
{
    ENTRY("distributeMessage(topic: " + topic + ", body: " + body + ")");
    auto entry = subscriptions.find(topic);
    if (entry != subscriptions.end())
    {
        auto subscribers = entry->second;
        for (auto iter = subscribers.begin(); iter != subscribers.end(); iter++)
        {
            auto subscriber = *iter;
            LOG("distributeMessage() - sending topic: " + topic + ", body: " + body);
            subscriber->messageReceived(topic, body);
        }
    }
    EXIT("distributeMessage()");
}

void MQTTBridge::sendMsg(const std::string &topic, const std::string &body)
{
    ENTRY("sendMsg(topic: " + topic + ", body: " + body + ")");
    auto buffer = std::make_shared<mqtt::buffer_ref<char>>(body.c_str());
    auto msg = std::make_shared<mqtt::message>(topic, *buffer);
    client->publish(msg);
    EXIT("sendMsg(topic: " + topic + ", body: " + body + ")");
}

void MQTTBridge::subscribe(const std::string &topic, MQTTMessageClient *client)
{
    ENTRY("subscribe(topic: " + topic + ")");
    std::set<MQTTMessageClient *> &entry = subscriptions[topic];
    entry.insert(client);
    EXIT("subscribe(topic: " + topic + ")");
}