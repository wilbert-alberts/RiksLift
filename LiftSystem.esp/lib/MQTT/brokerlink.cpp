//
// brokerLink.cpp -- build and maintain link to MQTT broker
//
// BSla, 2 Aug 2024
//

#include <Arduino.h>
#include <ETH.h>
#include <ESPping.h>
#include <MQTTPubSubClient.h>  // hideakitai 0.3.2
#include "timer.h"
#include "identity.h"
#include "brokerlink.h"

#define _DEBUG 1
#include "debug.h"

const int      MQTTPort      (1883);
const int      KeepAliveTime   (35);   // seconds
const uint32_t GuardTime (1 SECOND);
const uint32_t ResetTime (1 MINUTE);

BrokerLink brokerLink;
static MQTTPubSubClient mqttClient;

const IPAddress BrokerLink::IPLowerFloor   (192, 168, 9, 20);
const IPAddress BrokerLink::IPUpperFloor   (192, 168, 9, 30);
const IPAddress BrokerLink::IPMotorControl (192, 168, 9, 40);
const IPAddress BrokerLink::IPBroker       (192, 168, 9,  1);

static const char *lastWillTopic   ("clientstatus");
static const char *lastWillOnline  ("online");
static const char *lastWillOffline ("offline");

static const bool wait (true);

void BrokerLink::setup() {
  guardTimer.start(GuardTime);
  resetTimer.start (ResetTime);
  toState(AllUp, wait, true);
  loopCount = 0;
}

void BrokerLink::shutdown() {
   toState(AllDown, wait, false);
}

void BrokerLink::checkState ()
{
  const char *context = "*** BrokerLink::checkState: ";
  if (linkState == AllUp) {
     if (!MQTTIsUp (false)) {
        DEBUG("%sMQTT is down\n", context);
        linkState = ClientUp;
     }
  }
  if (linkState == ClientUp) {
     if (!clientIsUp(false)) {
        DEBUG("%snetworkClient is down\n", context);
        linkState = ETHUp;
     }
  }
  if (linkState == ETHUp) {
     if (!ETHIsUp(false)) {
        DEBUG("%sETH is down\n", context);
        linkState = AllDown;
     }
  }  
}

void BrokerLink::loop() 
{
   if (loopCount++ > 100) {  // to reduce avg loop time
      loopCount = 0;
      checkState ();
      if (linkState == AllUp) mqttClient.update ();        // to handle callbacks
   }
   if (linkState != wantedState) {
      DEBUG("   BrokerLink::loop: recovering lost link\n");
      toState(wantedState, wait, true);
   }
}

bool BrokerLink::toState(LinkState target, bool wait, bool verbose) {
  if (verbose) { DEBUG(">  BrokerLink::toState (target = %s, wait = %s)\n", ls2str(target), toCCP(wait)); }
  wantedState = target;
  bool r = false;
  do {
    LinkState oldState = linkState;
    r = handleStates(verbose);
    checkState ();
    bool stateChanged = (linkState != oldState);
    if (stateChanged) resetTimer.start ();
    if (wait && !stateChanged) {
      delay(GuardTime);
    }
    if (resetTimer.hasExpired ()) {
      DEBUG ("**** BrokerLink:: timer expired; restarting\n");
      delay (2000);
       ESP.restart ();
    }
  } while (wait && !r);
  if (verbose) { DEBUG("<  BrokerLink::toState () = %s\n", toCCP(r)); }
  return r;
}

void BrokerLink::showState (const char *context)
{
    DEBUG ("   BrokerLink:%s:linkState is now %s\n", context, ls2str(linkState));
}

void BrokerLink::showTransition (const char *context, LinkState from, LinkState to)
{
    DEBUG ("   BrokerLink:%s: %s to %s\n", context, ls2str(from), ls2str (to));
}

void BrokerLink::stepUp(bool verbose) 
{
  const char *context = "stepUp";
  switch (linkState) {
    case AllDown:
      if (verbose) showTransition (context, AllDown, ETHComingUp);
      if (startETH(verbose)) {
        linkState = ETHComingUp;
        showState (context);
      }
      break;
    case ETHComingUp:
      if (verbose) showTransition (context, ETHComingUp, ETHUp);
      if (ETHIsUp(verbose)) {
        linkState = ETHUp;
        showState (context);
      }
      break;
    case ETHUp:
      if (verbose) showTransition (context, ETHUp, ClientComingUp);
      if (startClient(verbose)) {
        linkState = ClientComingUp;
        showState (context);
      }
      break;
    case ClientComingUp:
      if (verbose) showTransition (context, ClientComingUp, ClientUp);
      if (clientIsUp(verbose)) {
        linkState = ClientUp;
        showState (context);
      }
      break;
    case ClientUp:
      if (verbose) showTransition (context, ClientUp, BrokerComingUp);
      if (startMQTT(verbose)) {
        linkState = BrokerComingUp;
        showState (context);
      }
      break;
    case BrokerComingUp:
      if (verbose) showTransition (context, BrokerComingUp, AllUp);
      if (MQTTIsUp(verbose)) {
        linkState = AllUp;
        showState (context);
      }
      break;
    case AllUp:
      DEBUG("   BrokerLink:stepUp:linkState is AllUp; we are done\n");
      break;
  }  // switch
}

void BrokerLink::stepDown(bool verbose) 
{
  const char *context = "stepDown";
  switch (linkState) {
    case AllUp:
    case BrokerComingUp:
      if (verbose) showTransition (context, linkState, ClientUp);
      if (stopMQTT(verbose)) {
        linkState = ClientUp;
        showState (context);
      }
      break;
    case ClientUp:
    case ClientComingUp:
      if (verbose) showTransition (context, linkState, ETHUp);
      if (stopClient(verbose)) {
        linkState = ETHUp;
        showState (context);
      }
      break;
    case ETHUp:
    case ETHComingUp:
      if (verbose) showTransition (context, linkState, AllDown);
      if (stopETH(verbose)) {
        linkState = AllDown;
        showState (context);
      }
      break;
    case AllDown:
      DEBUG("   BrokerLink:stepDown:linkState is AllDown; we are done\n");
      break;
  }
}

bool BrokerLink::handleStates(bool verbose) {
  if (verbose) { DEBUG(">  BrokerLink::handleStates ()\n"); }
  bool r = (linkState == wantedState);

  if (r) {
    if (verbose) { DEBUG("  handleStates: LinkState == wantedState = %s: done\n", ls2str(linkState)); }
  } else {
    goingUp = (int(wantedState) > int(linkState));
    if (verbose) { DEBUG("   handleStates: linkState = %s, wanted state = %s, goingUp = %s\n", ls2str(linkState), ls2str(wantedState), toCCP(goingUp)); }
    if (goingUp) stepUp  (verbose);
    else         stepDown(verbose);
    if (verbose) { DEBUG("<  BrokerLink::handleStates () = %s\n", toCCP(r)); }
  }
  return r;
}


bool BrokerLink::startETH(bool verbose) 
{
  if (verbose) { DEBUG(">  BrokerLink::startETH ()\n"); }
  IPAddress ip(*getOwnIP(verbose));
  IPAddress dns(192, 168, 9, 1);
  IPAddress gateway(192, 168, 9, 1);
  IPAddress subnet(255, 255, 255, 0);

  bool r = true;

  if (!ethClient.connected()) {
    if (verbose) { DEBUG("   Connecting ETH"); }
    ETH.begin();
    ETH.config(ip, gateway, subnet, dns);
    while (!ETH.linkUp()) {
      if (verbose) { DEBUG("."); }
      delay(GuardTime);
    }
    if (verbose) {DEBUG("\n   ETH connected!\n");}
  }
  if (verbose) { DEBUG("<  BrokerLink::startETH = %s\n", toCCP(r)); }
  return r;
}

bool BrokerLink::ETHIsUp(bool verbose) 
{
  if (verbose) {DEBUG (">  BrokerLink::ETHIsUp ()");}
  bool connStatus = ETH.linkUp();

  if (connStatus != ETHWasUp) {
    DEBUG("   At %d, ETH Connected = %s\n",  millis() / 1000, toCCP(connStatus));
    ETHWasUp = connStatus;
  }
  if (verbose) {DEBUG ("<  BrokerLink::ETHIsUp () = %s", toCCP (connStatus));}
  return connStatus;
}

bool BrokerLink::stopETH(bool verbose) {
  bool r = true;
  //  ethClient.stop(); // In order to stop: disconnect cable
  if (verbose) { DEBUG(">< BrokerLink::stopETH () = %s\n", toCCP(r)); }
  return r;
}

bool BrokerLink::startClient(bool verbose) 
{
  if (verbose) { DEBUG(">  BrokerLink::startClient ()"); }
  ethClient.stop();
  bool r;
  do {
    r = Ping.ping(IPBroker, 1);
    if (verbose && !r) {DEBUG ("   BrokerLink::startClient: Ping broker failed\n");}
    r = r && ethClient.connect(IPBroker, MQTTPort);
    if (!r) {
      if (verbose) { DEBUG("."); }
      if (!ETHIsUp(verbose)) break;
      delay (GuardTime);
    }
  } while (!r);

  if (verbose) {DEBUG("\n");}
  if (r && verbose) { DEBUG("   ethClient connected\n"); }
  if (verbose) { DEBUG("<  BrokerLink::startClient = %s\n", toCCP(r)); }
  return r;
}

bool BrokerLink::clientIsUp(bool verbose) {
  bool c = ethClient.connected();
  bool p = Ping.ping(IPBroker, 1);
  if (c != clientWasUp) {
    DEBUG("   At %d, clientIsUp = %s, ping = %s\n", millis() / 1000, toCCP(c), toCCP (p));
    clientWasUp = c;
  }
  if (verbose) {DEBUG ("   BrokerLink::clientIsUp = %s, ping status = %s\n", toCCP (c), toCCP (p));}
  return c;
}

bool BrokerLink::stopClient(bool verbose) {
  bool r = true;
  ethClient.stop();
  if (verbose) {DEBUG(">< BrokerLink::stopClient () = %s\n", toCCP(r));}
  return r;
}

bool BrokerLink::startMQTT(bool verbose) 
{
  if (verbose) {DEBUG(">  BrokerLink::startMQTT ()\n");}
  String id(identity.getOwnID());
  String lwt (getLastWillTopic(verbose));
  bool r(false);

  mqttClient.begin(ethClient);
  mqttClient.setKeepAliveTimeout(KeepAliveTime);
  mqttClient.disconnect();
  if (verbose) {DEBUG("   Connecting to MQTT broker, id = %s", id.c_str()); }
  do {
    r = mqttClient.connect(id);
    if (!r) {
      if (!clientIsUp(verbose)) break;  // network client is gone
      if (verbose) {DEBUG(".");}
      delay(GuardTime);
    }
  } while (!r);
  if (verbose) { DEBUG("\n"); }
  if (r) {
    if (verbose) { DEBUG("   mqttClient connected\n"); }
    mqttClient.setWill (lwt, String(lastWillOffline));
    r = mqttClient.publish (lwt, String(lastWillOnline));
    if (r) restoreSubs (verbose);
  }
  
  if (verbose) { DEBUG("<  BrokerLink::startMQTT () = %s\n", toCCP(r)); }
  return r;
}

bool BrokerLink::MQTTIsUp(bool verbose) {
  bool r = mqttClient.isConnected();
  if (r != mqttWasUp) {
    DEBUG("   MQTTIsUp: At %d, mqttClient is %sconnected\n", millis() / 1000, r ? "" : "NOT ");
    mqttWasUp = r;
  }
  if (verbose) { DEBUG (">< BrokerLink::MQTTIsUp () = %s\n", toCCP (r));}
  return r;
}

bool BrokerLink::stopMQTT(bool verbose) {
  bool r = mqttClient.disconnect();
  if (verbose) { DEBUG(">< BrokerLink::stopMQTT () = %s\n", toCCP(r)); }
  return r;
}

const IPAddress *BrokerLink::getOwnIP (bool verbose) 
{
  if (verbose) { DEBUG(">   BrokerLink::GetOwnIP ()\n"); }
  const IPAddress *result = &IPMotorControl;
  if (!identity.isFloorController()) {
    result = &IPMotorControl;
  } else if (identity.isUpperFloor()) {
    result = &IPUpperFloor;
  } else {
    result = &IPLowerFloor;
  }

  if (verbose) { DEBUG("<   BrokerLink::getOwnIP: IP = %s\n", ip2str(*result).c_str()); }
  return result;
}

String BrokerLink::getLastWillTopic(bool verbose) 
{
  if (verbose) { DEBUG(">  BrokerLink::getLastWillTopic()\n");}
  Topic topic (identity.getOwnID ()); topic += lastWillTopic;
  if (verbose) { DEBUG("<  BrokerLink::getLastWillTopic() = %s\n", topic.get ().c_str()); }
  return topic.get ();
}

bool BrokerLink::publish(String &topic, const String &payload, bool verbose) 
{
  if (verbose) { DEBUG(">  BrokerLink::publish(topic: %s, payload: %s)\n", topic.c_str(), payload.c_str()); }
  bool result (false);
  if (isUp ()) result = mqttClient.publish(topic, payload);
  if (verbose) { DEBUG("<  BrokerLink::publish() = %s\n", toCCP(result)); }
  return result;
}

bool BrokerLink::subscribe (const globalCallback_t& cb, bool verbose)
{
  if (verbose) {DEBUG(">  BrokerLink::subscribe global ()\n"); }
  bool r = globalSubStore.storeSub (cb, verbose);
  r = r &&  isUp ();
  if (r) mqttClient.subscribe(cb);
  if (verbose) { DEBUG("<  BrokerLink::subscribe global() = %s\n", toCCP (r)); }
  return r;
}

bool BrokerLink::subscribe (String& topic, const topicCallback_t &cb, bool verbose)
{
  if (verbose) {DEBUG(">  BrokerLink::subscribe (topic = %s)\n", topic.c_str ()); }
  bool r = topicSubStore.storeSub (topic, cb, verbose);
  r = r && isUp ();
  if (r) mqttClient.subscribe(topic, cb);
  if (verbose) { DEBUG("<  BrokerLink::subscribe (%s) = %s\n", topic.c_str (), toCCP(r)); }
  return r;
}

void BrokerLink::restoreSubs (bool verbose)
{
   if (verbose) {DEBUG (">  BrokerLink::restoreSubs ()\n");}
   bool r = true;
   String           topic;
   topicCallback_t  tcb;
   globalCallback_t gcb;
   topicSubStore.reset ();  
   while (r) {
      r = topicSubStore.getNextSub (topic, tcb, verbose);
      if (r) {
         if (verbose) {DEBUG ("   Subscribing topic %s\n", topic.c_str ());}
         mqttClient.subscribe  (topic, tcb);
      }
   }
   r = true;
   globalSubStore.reset ();  
   while (r) {
      r = globalSubStore.getNextSub (gcb, verbose);
      if (r) {
         if (verbose) {DEBUG ("   Subscribing global callback\n");}
         if (r) mqttClient.subscribe   (gcb);
      }
   }
   if (verbose) {DEBUG ("<  BrokerLink::restoreSubs ()\n");}
}

const char *BrokerLink::ls2str(LinkState ls) {
  static const char *ad = "AllDown";
  static const char *ecu = "ETHComingUp";
  static const char *eu = "ETHUp";
  static const char *ccu = "ClientComingUp";
  static const char *cu = "ClientUp";
  static const char *bcu = "BrokerComingUp";
  static const char *au = "AllUp";
  return (ls == AllDown        ? ad 
        : ls == ETHComingUp    ? ecu
        : ls == ETHUp          ? eu
        : ls == ClientComingUp ? ccu
        : ls == ClientUp       ? cu
        : ls == BrokerComingUp ? bcu
        :                         au);
}

String BrokerLink::ip2str(IPAddress ip) {
  String s;
  for (int i = 0; i < 4; i++) {
    s += int(ip[i]);
    if (i < 3) s += '.';
  }
  return s;
}

bool TopicSubStore::getNextSub (String &topic, topicCallback_t &cb, bool verbose)
{
   if (verbose) {DEBUG (">  TopicSubStore::getNextSub ()\n");}
   bool r = (currentSub < usedSubs);
   if (r) {
      TopicSub &store = theSubs [currentSub++];
      topic = store.topic;
      cb = store.cb;
   }
   if (verbose) {DEBUG ("<  TopicSubStore::getNextSub (topic %s) = %s\n", topic.c_str (), toCCP (r));}
   return r;
}

bool TopicSubStore::storeSub (const String &topic, const topicCallback_t &cb, bool verbose)
{
   if (verbose) {DEBUG (">  TopicSubStore::storeSub (topic %s)\n", topic.c_str ());} 
   bool r = (usedSubs < nTopicSubs);
   if (r) {
      TopicSub &store = theSubs [usedSubs++];
      store.topic = topic; 
      store.cb = cb;
   }
   else {
      DEBUG ("*** TopicSubStore: storeSub: no more storage\n");
   }
   if (verbose) {DEBUG ("<  TopicSubStore::storeSub () = %s\n", toCCP (r));}
   return r;
}

bool GlobalSubStore::getNextSub (globalCallback_t &cb, bool verbose)
{
   if (verbose) {DEBUG (">  GlobalSubStore::getNextSub ()\n");}
   bool r = (currentSub < usedSubs);
   if (r) {
      GlobalSub &store = theSubs [currentSub++];
      cb = store.cb;
   }
   if (verbose) {DEBUG ("<  GlobalSubStore::getNextSub () = %s\n", toCCP (r));}
   return r;
}

bool GlobalSubStore::storeSub (const globalCallback_t &cb, bool verbose)
{
   if (verbose) {DEBUG (">  GlobalSubStore::storeSub ()\n");}
   bool r = (usedSubs < nGlobalSubs);
   if (r) {
      GlobalSub &store = theSubs [usedSubs++];
      store.cb = cb;
   }
   else {
      DEBUG ("*** GlobalSubStore: storeSub: no more storage\n");
   }
   if (verbose) {DEBUG ("<  GlobalSubStore::storeSub () = %s\n", toCCP (r));}
   return r;
}