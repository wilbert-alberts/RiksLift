//
// Simulation.h -- definition of simulation class
//
// BSla, 28 Jun 2024
#ifndef _SIMULATION_H
#define _SIMULATION_H
#include <Arduino.h>
#define SIMULATION 1  // 1 for sim on, 0 for sim off

#include "hsi.h"
#include "liftencoder.h"




class Simulation {
#if SIMULATION==1
  public:
    Simulation () {}
    ~Simulation () {}
    void setup ();
    void loop ();

    // endstops
    bool isBelowPhysicalLower ();
    bool isAbovePhysicalUpper ();
    void simulateSlip ();
    bool isAboveUpperGoingUp ();
    bool isAboveUpperGoingDown ();
    bool isAboveUpper ();
    bool isBelowLowerGoingUp ();
    bool isBelowLowerGoingDown ();
    bool isBelowLower ();

    //liftencoder
    float getPosition ();
    bool setOffset (float _offset);

    // VFD
   void setEmoActive (bool active) {emoActive = active;}
   bool isEmoActive () {return emoActive;}
   bool isPowered ();
//   bool startMove (bool up, uint8_t Hz);
   bool setSpeed (uint8_t _pwm);
   bool brake (); // Returns: TRUE means brake complete
   void forward (bool f);
   void reverse (bool r);
   bool reset (bool x);

  private:
   bool emoActive;  
   int32_t position = 0;   // used for simulation; represents sensor readout
   int32_t previousPosition = LiftEncoder::toIncrements (10.0);
   int32_t offset = 0;     // remember offset
   int8_t pwm = 0;        // current pwm value
   bool movingForward;
   bool previousForward;
   bool movingReverse;
   bool previousReverse;
   bool oldAboveUpper;
   bool oldBelowLower;
   bool speedWarningGiven;
   bool posWarningGiven;
   int32_t loopCount;

#elif SIMULATION==0
#else
#error SIMULATION not defined as 0 or 1 in simulation.h
#endif

};
extern Simulation sim;

#endif