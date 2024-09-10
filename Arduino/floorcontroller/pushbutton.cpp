//
// pushbutton.cpp -- tricolor LED lit pushbutton
//
// BSla, 7 Jul 2024
//
#include "hsi.h"
#include "liftglobals.h"
#include "timer.h"
#include "BrokerLink.h"
#include "identity.h"

#define _DEBUG 1
#include "debug.h"

#include "pushbutton.h"

#define FLASH_PERIOD        (700 MILLISECONDS)
#define ALARM_FLASH_PERIOD  (300 MILLISECONDS)

LitPushbutton pushbutton (GPIO_BTN_PUSH, GPIO_BTN_LED_RED, GPIO_BTN_LED_GREEN, GPIO_BTN_LED_BLUE, "CallButton");
Pushbutton emo (GPIO_BTN_EMO, DEBOUNCE_TIME, "EmergencyOff");
Pushbutton screen (GPIO_SCREEN, 1, "Lightscreen");

void Pushbutton::setup ()
{
   pinMode (pbGPIO, INPUT);
   wasPushed = true;
   followPushed = true;
   publishTimer.start (REPORT_INTERVAL);
   debounceTimer.start ();
   isPushed ();
}

void Pushbutton::report (bool pushed)
{
  Topic topic; String value;
  if (this == &pushbutton) {
     topic.set (mqtButton); topic += identity.whichFloor(); topic += mqsStatus;
     value = pushed ? mqsPressed : mqsReleased;
  }
  else if (this == &emo) {
     topic.set (mqtEmergencyButton); topic += identity.whichFloor(); topic += mqsStatus;
     bool iP = isPushed ();
     value = iP ? mqsTriggered : mqsCleared;
     DEBUG (">  Pushbutton %s report topic = %s, value = %s\n", name, topic.get ().c_str (), value.c_str ());
     brokerLink.publish (topic, value, false);

     topic.set (mqtIntrusion); topic += identity.whichFloor (); topic += mqsStatus;
     // trick to pass emo events as screen events to RL logic
     bool emoOrScreenActive = iP || screen.isPushed ();
     value = emoOrScreenActive ? mqsTriggered : mqsCleared;     
  }
  else if (this == &screen) {
     topic.set (mqtIntrusion); topic += identity.whichFloor (); topic += mqsStatus;
     // trick to pass emo events as screen events to RL logic
     bool emoOrScreenActive = isPushed () || emo.isPushed ();
     value = emoOrScreenActive ? mqsTriggered : mqsCleared;    
  }
  DEBUG (">  Pushbutton %s report topic = %s, value = %s\n", name, topic.get ().c_str (), value.c_str ());
  bool r = brokerLink.publish (topic, value, false);
  DEBUG ("<  Pushbutton::report () = %s\n", toCCP (r));
}

bool Pushbutton::isPushedBouncing ()
{
   return ((!digitalRead (pbGPIO)) || simPushed);
}

bool Pushbutton::isPushed ()
{
   while (debounceTimer.isActive ()) {
      bool ipb = isPushedBouncing ();
      if (ipb != followPushed) {
         debounceTimer.start ();
         followPushed = ipb;
      }
   }
   return followPushed;
}

void Pushbutton::loop ()
{
   bool ipb = isPushedBouncing ();
   if (ipb != followPushed) {
      debounceTimer.start ();
      followPushed = ipb;
   }

   bool iP = isPushed ();
   if (iP != wasPushed) {
      wasPushed = iP;
      DEBUG ("   %s::loop: changed state; is now %s\n", name, iP?"pushed": "released");
      report (iP);
      publishTimer.start (REPORT_INTERVAL);
   }
   if (publishTimer.hasExpired ()) {
     report (iP);
     publishTimer.restart ();
   }
}

LitPushbutton::LitPushbutton (uint8_t theBtn, uint8_t lRed, uint8_t lGreen, uint8_t lBlue, const char *_name): 
   Pushbutton (theBtn, DEBOUNCE_TIME, _name)
{
   for (int i = 0; i < 3; i++) {
      rgb [i].gpio = (i==0)? lRed: (i==1)? lGreen: lBlue;
   }
}

void LitPushbutton::setup ()
{
   Pushbutton::setup ();
   alarmCount = 0;
   wasAlarming = false;
   ledColor = None;  
   isFlashing = false;
   for (int i = 0; i < 3; i++) {
      rgb [i].isOn = false;
      rgb [i].isFlashing = false;
      pinMode (rgb[i].gpio,  OUTPUT);
      led &thisLED = rgb[i];
      digitalWrite (thisLED.gpio, 1);
   }
   flashTimer.start (FLASH_PERIOD);
}

void LitPushbutton::loop ()
{
   Pushbutton::loop ();
   if (flashTimer.hasExpired ()) {
      for (int i = 0; i < 3; i++) {
         led &thisLED = rgb[i];
         if (thisLED.isFlashing) {
            thisLED.isOn = !thisLED.isOn;
            digitalWrite (thisLED.gpio, !thisLED.isOn);
         }
      }
      flashTimer.restart ();
   }
}

void LitPushbutton::alarm (bool on)
{
  if (on) {
     if (alarmCount == 0) {
       writeLED (Red, true);
       flashTimer.start (ALARM_FLASH_PERIOD);
     }
     alarmCount++;
  }
  else {
     if (alarmCount > 0) alarmCount--;
     if (alarmCount == 0) {
        writeLED (ledColor, isFlashing);
        flashTimer.start (FLASH_PERIOD);
     }
  }
}

void LitPushbutton::setLED (LedColor led, bool flashing)
// set led. May be overruled by alarm
{
   ledColor = led;
   isFlashing = flashing;
   if (alarmCount == 0) writeLED (led, flashing);
}

void LitPushbutton::writeLED (LedColor ledColor, bool flashing)
{
   for (int i = 0; i < 3; i++) {
      led &thisLED = rgb[i];
      if (i == int(ledColor)) {
        thisLED.isOn       = true;
        thisLED.isFlashing = flashing;
      }
      else {
        thisLED.isOn       = false;
        thisLED.isFlashing = false;
      }
      digitalWrite (thisLED.gpio, !thisLED.isOn);
   }
}

void LitPushbutton::setLED (LiftState ls)
{
   bool isUpper = identity.isUpperFloor ();
   bool moving;
   bool up;
   bool lit;
   bool alarming;
   switch (ls) {
     case lsMovingUp:   moving = true;  up = true;  lit = true;  alarming = false; break;
     case lsMovingDown: moving = true;  up = false; lit = true;  alarming = false; break;
     case lsUp:         moving = false; up = true;  lit = true;  alarming = false; break;
     case lsDown:       moving = false; up = false; lit = true;  alarming = false; break;
     case lsStandstill: moving = false; up = false; lit = true;  alarming = false; break;
     default:           moving = false; up = false; lit = false; alarming = true;  break;
   }

   alarm (alarming);

   if (!alarming) {
      LedColor color = (up == isUpper? Green:Red);  // green if moving towards, red if moving away from
      
      if (lit) setLED (color, moving);  // color, flashing
      else  setLED (None,  false);
   }
}
