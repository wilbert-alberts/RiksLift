//
// vevorVFD.h -- control for VEVOR VFD for motor controller for Riks Lift
//
// HSI:
// pin GPIO purpose 
//  9L   4  VEVOR powered on (active low)
//  7R  16  Forward (active high) moves lift DOWN
//  8R  15  Reverse (active high) moves lift UP
//  9R  14  Reset (active high)
// 10R  13  Motor speed (pwm, active high)
#ifndef _VEVORVFD_H
#define _VEVORVFD_H

#include <Arduino.h>
#include "Simulation.h"


// speeds in Hz
#define VFD_STILL   (   0 )
#define VFD_LOW     (  25 )    
#define VFD_MEDIUM  (  46 )
#define VFD_HIGH    (  92 )

class VevorVFD {
  public:
   VevorVFD  () {}
   ~VevorVFD () {}
   bool setup ();
   void loop ();
   bool isPowered ();
   bool startMove (bool up, uint8_t Hz);
   bool setSpeed (uint8_t Hz, bool reduceOnly);
   bool brake (); // Returns: TRUE means brake complete
   bool reset ();
  private:   
   void pForward (bool f);
   void pReverse (bool f);
   void pReset (bool f);
   void pSetSpeed (uint8_t pwm);
   uint8_t HzToPWM (uint8_t Hz);
   void delay (uint32_t millis);

   uint8_t pwd, fwd, rev, res, speedOut;
   uint8_t currentPWM; // as PWM fraction (0..255)
   bool reportedP;
   bool movingUp;
   bool movingDown;
#if SIMULATION==1
   Simulation *simPtr;
#endif
};

extern VevorVFD vevorVFD;
#endif