//
// Simulation.h -- definition of simulation class
//
// BSla, 28 Jun 2024
#ifndef _SIMULATION_H
#define _SIMULATION_H
#include <Arduino.h>
#include <timer.h>
#include "hsi.h"

#define SIMULATION 1 // 1 for sim on, 0 for sim off

class Simulation;

class Simulation
{
#if SIMULATION == 1
public:
   Simulation() {}
   ~Simulation() {}
   void setup (bool initPosition = true);
   void loop();

   // endstops
   bool isBelowPhysicalLower();
   bool isAbovePhysicalUpper();
   void simulateSlip();
   bool isAboveUpperGoingUp();
   bool isAboveUpperGoingDown();
   bool isAboveUpper();
   bool isBelowLowerGoingUp();
   bool isBelowLowerGoingDown();
   bool isBelowLower();

   // liftencoder
   float getPosition();                 // meters in client coords 
   bool  setOffset(float _offset);
   void  setToUpperEndstop ();
   void  setToLowerEndstop ();



   // VFD
   void setEmoActive(bool active) { emoActive = active; }
   bool isEmoActive() { return emoActive; }
   bool isPowered();

   bool setSpeed(uint8_t _pwm);
   bool brake(); // Returns: TRUE means brake complete
   void forward(bool f);
   void reverse(bool r);
   bool reset(bool x);

private:
   float toMeterPerMS (uint8_t Hz);
   bool initEverDone = false;
   bool emoActive;
   float position; // used for simulation; represents sensor readout
   float previousPosition;
   float offset; // remember offset
   uint8_t speed = 0;     // current pwm value
   float meterPerMS;
   uint32_t previousMillis;
   bool movingForward;
   bool previousForward;
   bool movingReverse;
   bool previousReverse;
   bool oldAboveUpper;
   bool oldBelowLower;
   bool speedWarningGiven;
   bool posWarningGiven;
   int32_t loopCount;
   //Timer reportTimer;

#elif SIMULATION == 0
#else
#error SIMULATION not defined as 0 or 1 in simulation.h
#endif
};
extern Simulation sim;

#endif