//
// display.h -- handle display
//
// BSla, 11 Jul 2024

#ifndef _DISPLAY_H
#define _DISPLAY_H

#include <Arduino.h>
#include "hsi.h"
#include "SSD1306Wire.h"
#include "Timer.h"
#include "liftglobals.h"


class Display {
  public:
   Display () {}
   ~Display () {}
   void setup ();
   void loop (); // for flashing
   void showState (LiftState ls);                // show lift state
   void setMoveTime (int seconds) {countdownTime = seconds; showState (liftState);}
   void showMessage (const String &message);
   void clearMessage ();                         // remove message; show state instead

  private:
   enum Symbol    {UpArrow, DownArrow, Cross, Box, None};
   LiftState liftState;
   bool   messagePresent;
   bool   isOn;
   bool   flashing;
   Timer  flashTimer;
   Timer  countdownTimer;
   int    countdownTime;
   Symbol displayedSymbol;

   void drawArrow (bool up, int number);
   void drawBox ();
   void drawCross ();
   void clear ();
   void drawSymbol (Symbol symbol);
   void keepTimeEstimate (bool start);
   static SSD1306Wire ssd13;
};

extern Display display;
#endif