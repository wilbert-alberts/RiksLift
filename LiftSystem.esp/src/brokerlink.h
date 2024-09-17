//
// brokerLink.h -- build and maintain link to MQTT broker
//
// BSla, 2 Aug 2024
//
#ifndef _BROKERLINK_H
#define _BROKERLINK_H

#ifndef ETH_PHY_TYPE
#define ETH_PHY_TYPE ETH_PHY_LAN8720
#define ETH_PHY_ADDR 0
#define ETH_PHY_MDC 23
#define ETH_PHY_MDIO 18
#define ETH_PHY_POWER -1
#define ETH_CLK_MODE ETH_CLOCK_GPIO0_IN
#endif

#include <ETH.h>
#include <WiFiClient.h>

#include <Arduino.h>
#include "timer.h"
#include <MQTTPubSubClient.h>

typedef void (*globalCallback_t) (const String &topic, const String &value, const size_t size); 
typedef void (*topicCallback_t)  (const String &value, const size_t size); 

class Topic {
public:
   Topic (): topic () {}
   Topic (const String &s): topic (s) {}
   Topic &add (const String &s) {topic += '/'; topic += s; return *this;}
   Topic &operator += (const String &s) {return add (s);}
   String &get () {return topic;}
   void set (String s) {topic = s;}
private: 
   String topic;
};



struct TopicSub {
   String topic;
   topicCallback_t cb; 
};

struct GlobalSub {
   globalCallback_t cb;
};

const int nGlobalSubs = 4;
const int nTopicSubs = 30;

class TopicSubStore {
 public: 
   TopicSubStore ():usedSubs (0), currentSub (0) {}
   void reset () {currentSub = 0;}
   void clear () {usedSubs = currentSub = 0;}
   bool getNextSub (String &topic, topicCallback_t &cb, bool verbose);
   bool storeSub   (const String &topic, const topicCallback_t &cb, bool verbose);
 private:
   int usedSubs;
   int currentSub;
   TopicSub theSubs [nTopicSubs]; 
};

class GlobalSubStore {
 public:
   GlobalSubStore ():usedSubs (0), currentSub (0) {}
   void reset () {currentSub = 0;}
   void clear () {usedSubs = currentSub = 0;}
   bool getNextSub (globalCallback_t &cb, bool verbose);
   bool storeSub   (const globalCallback_t &cb, bool verbose);
 private:
   int usedSubs;
   int currentSub;
   GlobalSub theSubs [nGlobalSubs]; 
};





class BrokerLink {
  public:
   enum LinkState {AllDown, ETHComingUp, ETHUp, ClientComingUp, ClientUp, BrokerComingUp, AllUp};
   BrokerLink () {}
   ~BrokerLink () {}
   void setup    (); // setup the link (and wait for completion)
   void shutdown (); // shutdown the link
   void loop ();     // maintain
   bool isUp () {checkState (); return (linkState == AllUp);}     // check if link is up
   bool publish (String &topic, const String &payload, bool verbose);
   bool publish (Topic &topic, const String &payload, bool verbose) {return publish (topic.get (), payload, verbose);}
   bool subscribe (const globalCallback_t& cb, bool verbose);
   bool subscribe (String& topic, const topicCallback_t &cb, bool verbose);
   bool subscribe (Topic&  topic, const topicCallback_t &cb, bool verbose) {return subscribe (topic.get (), cb, verbose);}

  private:
   Timer guardTimer;
   Timer resetTimer;
   LinkState linkState   = AllDown;
   LinkState wantedState = AllDown;
   bool goingUp = false;
   bool ETHWasUp = false;
   bool clientWasUp = false;
   bool mqttWasUp = false;
   NetworkClient ethClient;
   static const IPAddress IPLowerFloor;
   static const IPAddress IPUpperFloor;
   static const IPAddress IPMotorControl;
   static const IPAddress IPBroker;
   TopicSubStore topicSubStore;
   GlobalSubStore globalSubStore;
   int  loopCount;

   void checkState ();
   bool startETH (bool verbose);
   bool ETHIsUp (bool verbose);
   bool stopETH (bool verbose);
   bool startClient (bool verbose);
   bool clientIsUp (bool verbose);
   bool stopClient (bool verbose);
   bool startMQTT (bool verbose);
   bool MQTTIsUp (bool verbose);
   bool stopMQTT (bool verbose);
   void restoreSubs (bool verbose);
   String getLastWillTopic (bool verbose);
   bool toState (LinkState target, bool wait, bool verbose);
   void stepUp (bool verbose);
   void stepDown (bool verbose);
   bool handleStates (bool verbose);
   void showState (const char *context);
   void showTransition (const char *context, LinkState from, LinkState to);
   const IPAddress* getOwnIP (bool verbose); 
   const char *ls2str (LinkState ls);
   String ip2str (IPAddress ip);
};

extern BrokerLink brokerLink;
#endif