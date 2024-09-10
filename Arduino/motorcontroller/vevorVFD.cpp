// 
// vevorVFD.cpp -- VEVOR VFD control functions
//
// BSla, 14 jun 2024
#include "vevorVFD.h"

#define _DEBUG 1
#include "Debug.h"
#include "timer.h"
#include "simulation.h"

#define BRAKE_DELAY    (50 MILLISECONDS)
#define RESET_ON_DELAY (500 MILLISECONDS)
#define RESET_DELAY    (2 SECONDS)


VevorVFD vevorVFD;

#if _DEBUG==1
static const char* sfToStr (bool s)
{
    return s?"success":"***** fail";
}
#endif


bool VevorVFD::setup ()
{
  DEBUG (">  Vevor VFD:: setup\n");
  pwd = GPIO_VFD_POWERED; fwd = GPIO_VFD_FORWARD; rev = GPIO_VFD_REVERSE; res = GPIO_VFD_RESET; speedOut = GPIO_VFD_SPEED;
#if SIMULATION==0
  pinMode (pwd, INPUT); pinMode (fwd, OUTPUT); pinMode (rev, OUTPUT);
  pinMode (res, OUTPUT); pinMode (speedOut, OUTPUT);
  digitalWrite (fwd, 0); digitalWrite (rev, 0); digitalWrite (res, 0);
#endif
  currentPWM = 0;
#if SIMULATION==0
  analogWrite (speedOut, 0);
  analogWriteFrequency (speedOut, 1000);
#else // simulation
  simPtr = sim.getSimPtr ();
#endif
  bool p = isPowered ();
  if (p) DEBUG ("   VevorVFD::setup: VFD is powered.\n");
  DEBUG ("<  Vevor VFD::setup done\n");
  return p;
}

bool VevorVFD::startMove (bool up, uint8_t Hz)
{
   DEBUG (">  VevorVFD: startMove %s, %d Hz\n", up?"up":"down", Hz);
   bool p = isPowered ();
   if (p) {
      bool u = up; bool d = !up; bool r = false;
      if ((u != movingUp) || (d != movingDown)) {
         // change of direction
         brake ();
      }
      pForward (d); 
      pReverse (u);
      pReset (r);
      movingUp = u; movingDown=d;
      setSpeed (Hz, false);
   }
   DEBUG ("<  VevorVFD: startMove %s\n", p? "success":"fail");
   return p;
}


bool VevorVFD::setSpeed (uint8_t Hz, bool reduceOnly)
{
   bool p = isPowered ();
   if (p) {
      uint8_t pwm = HzToPWM (Hz);
      bool change = (currentPWM != pwm);
      if (reduceOnly) change = (currentPWM > pwm);
      if (change) {
         DEBUG (">< VevorVFD:: setSpeed %d Hz, reduce = %s: %s\n", Hz, toCCP (reduceOnly), p? "success":"fail");
         currentPWM = pwm;
         pSetSpeed (pwm);
      }
   }
   return p;
}

void VevorVFD::pForward (bool f)
{
#if SIMULATION==0
   digitalWrite (fwd, f);
#else
   simPtr -> forward (f); 
#endif
  //DEBUG (">< pForward: f = %s\n", f?"true":"false");
}

void VevorVFD::pReverse (bool r)
{
#if SIMULATION==0
   digitalWrite (rev, r);
#else
   simPtr -> reverse (r);
#endif
 // DEBUG (">< pReverse: r = %s\n", r?"true":"false");
}

void VevorVFD::pReset (bool x)
{
#if SIMULATION==0
   digitalWrite (res, x);
#else
   simPtr -> reset (x);
#endif
 // DEBUG (">< Reset = %s\n", x?"true":"false");
}

void VevorVFD::pSetSpeed (uint8_t pwm)
{
#if SIMULATION==0
   analogWrite (speedOut, pwm);
#else
   simPtr -> setSpeed (pwm);
#endif  
}


bool VevorVFD::brake ()
{
   bool p = isPowered ();
   bool doDelay = (movingUp || movingDown) && (currentPWM != 0);

   // switch everything off
   setSpeed (0, false);
   movingUp = false; movingDown = false; currentPWM = 0;
   pForward (false);
   pReverse (false);
   pReset   (false);
   if (p) {
      if (doDelay) {
         //DEBUG (">< Vevor VFD::brake \n");
         delay (BRAKE_DELAY);
      }
   }
   if (!p) DEBUG ("**** VevorVFD::brake FAILED; was not powered\n");
   return p;
}


bool VevorVFD::reset ()
{
   DEBUG (">  VevorVFD::reset\n");
   bool p = isPowered ();
   pForward (false);
   pReverse (false);
   pSetSpeed (0);
   if (p) {
      pReset (true);
      delay (RESET_ON_DELAY);
      pReset (false);
      delay (RESET_DELAY);
      pForward (movingDown);
      pReverse (movingUp);
      pSetSpeed (currentPWM); 
   }
   DEBUG ("<  VevorVFD::reset: %s\n", sfToStr (p));
   return p;
}

bool VevorVFD::isPowered ()
{
   bool powered = false;
#if SIMULATION==0
   powered = !digitalRead (pwd);
#elif SIMULATION==1
   powered = !simPtr->isEmoActive ();
#endif 
   if (powered != reportedP) {
      DEBUG ("   Vevor VFD is %spowered\n", powered?"":"NOT ");
      reportedP = powered;
   }
   return powered;
}

uint8_t VevorVFD::HzToPWM (uint8_t Hz)
// based on 92Hz max freq
{
   uint8_t pwm;
#if SIMULATION==0
   // PWMs for        0, 10, 20, 30, 40,  50,  60,  70,  80,  90, 100.. Hz
   int pwmTable [] = {0, 26, 42, 65, 87, 111, 135, 158, 183, 206, 236 };

   if (Hz >= 92) pwm = 255;
   else {
      int tens = Hz / 10;
      int tensV  = pwmTable [tens];
      int tensV1 = pwmTable [tens + 1];
      int rest = Hz % 10;
      pwm =  uint8_t (tensV + (((tensV1-tensV) * rest) / 10));
   }
#elif SIMULATION==1
   pwm = Hz;
#endif
   return pwm;
}

void VevorVFD::delay (uint32_t millis)
{
    //DEBUG (">< VevorVFD: delay %ld milliseconds\n", millis);
#if SIMULATION==0
    Timer t;
    t.delay (millis);
#endif    
}
