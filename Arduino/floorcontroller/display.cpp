//
// display.cpp -- floor controller display handler
//
// BSla, 11 Jul 2024
//
#include "identity.h"
#include "timer.h"
#define _DEBUG 1
#include "debug.h"

#include "display.h"

const float filterTC = 0.3;

// is static in class Display
SSD1306Wire Display::ssd13 (ADDRESS_1306, SDA_1306, SCL_1306);

Display display;

void Display::setup () 
{
   liftState       = lsNone;
   messagePresent  = false;
   isOn            = false;
   flashing        = false;
   flashTimer.start (700 MILLISECONDS);
   countdownTimer.start (1 SECOND);
   countdownTime   = 0;
   displayedSymbol = None;

  ssd13.init();
  ssd13.flipScreenVertically();
  flashTimer.stop ();
  countdownTimer.stop ();
}

void Display::loop ()
{
   bool redraw = false;
   if (!flashing) isOn = true;
   else if (flashTimer.hasExpired ()) {
      isOn = !isOn;
      redraw = true;
      flashTimer.restart ();
   }

   if (countdownTimer.hasExpired ()) {
      countdownTimer.stop ();
      if (countdownTime != 0) {
         countdownTime --;
         redraw = true;
         countdownTimer.restart ();
      }
   }
   if (redraw && !messagePresent) drawSymbol (isOn? displayedSymbol: None);
}

void Display::showMessage (const String &message)
{
   messagePresent = true;
  
   int space1 = message.indexOf (' ');        // assumes a message consisting of 3 words 
   int space2 = message.lastIndexOf (' ');

   String line1 (message.substring (0, space1));
   String line2 (message.substring (space1+1, space2));
   String line3 (message.substring (space2+1));

   ssd13.clear ();
   ssd13.setFont (ArialMT_Plain_16);
   ssd13.setTextAlignment (TEXT_ALIGN_CENTER);

   ssd13.drawString (64,  0, line1);
   ssd13.drawString (64, 21, line2);
   ssd13.drawString (64, 42, line3);
   ssd13.display();
}

void Display::clearMessage ()
{
   messagePresent = false;
   drawSymbol (isOn? displayedSymbol: None);
}

void Display::showState (LiftState ls)
{
   liftState = ls;

   switch (liftState) {
    case lsMovingUp:
    case lsMovingDown:
      flashing = false;
      displayedSymbol = (liftState == lsMovingUp?UpArrow:DownArrow);
      if (!messagePresent) drawSymbol (displayedSymbol);
      countdownTimer.start ();
     break;

    case lsUp: 
    case lsDown:
      displayedSymbol = Cross; // preset: other floor
      if (((liftState == lsUp)   && ( identity.isUpperFloor ()))  ||
          ((liftState == lsDown) && (!identity.isUpperFloor ()))    ) {
         // the lift is at this floor
         displayedSymbol = Box;
      }
      flashing = false;
      flashTimer.stop ();
      if (!messagePresent)  drawSymbol (displayedSymbol);
     break;

    case lsStandstill:
      displayedSymbol = Cross;
      flashing = false;
     break;
    
    case lsNone:
      displayedSymbol = None;
      if (!messagePresent) drawSymbol (displayedSymbol);
      flashing = false;
     break;
   }
   if (!messagePresent) drawSymbol (displayedSymbol);
}

void Display::drawArrow (bool up, int number)
{
   ssd13.clear ();
   {  // draw triangle
      int wd = ssd13.getWidth ();
      int wd2 = wd / 2;
      int ht = ssd13.getHeight ();
      int ht2 = ht / 2;

      for (int y = 0; y < ht2; y++) {
         int x1 = wd2 - y;
         int x2 = x1  + 2*y;
         int yd = up?y:ht-y-1;
         ssd13.drawLine (x1,yd, x2,yd);
      }

   }
   if (number > 0) {
      // display the number
      ssd13.setFont (ArialMT_Plain_24);
      ssd13.setTextAlignment (TEXT_ALIGN_CENTER);

      int y = up? 33:0;

      ssd13.drawString (64,  y, String (number));
   }
   ssd13.display ();
}

void Display::drawCross ()
{
   const int thk = 10;
   const int wd = ssd13.getWidth ();
   const int ht = ssd13.getHeight ();

   ssd13.clear ();
   for (int w = 0; w < thk; w++) {
      ssd13.drawLine (w,0, wd-1,ht-1-w/2);
      ssd13.drawLine (0, ht-1-w/2, wd-1-w, 0);
      ssd13.drawLine (0, w/2, wd-1-w, ht-1);
      ssd13.drawLine (w, ht-1, wd-1, w/2);
   }
   ssd13.display ();
}

void Display::drawBox ()
{
   const int wd = ssd13.getWidth ();
   const int ht = ssd13.getHeight ();

   const int fex = 20;
   const int fey = 4;

   const int t = 2;

   // show a simplified image of the lift carriage

   ssd13.clear ();
   // 3 horizontal lines
   ssd13.drawRect (fex, fey,        wd-2*fex, t);
   ssd13.drawRect (fex, fey+t,      wd-2*fex, t);
   ssd13.drawRect (fex, ht/2-t,     wd-2*fex, t);
   ssd13.drawRect (fex, ht/2,       wd-2*fex, t);
   ssd13.drawRect (fex, ht-fey-2*t, wd-2*fex, t);
   ssd13.drawRect (fex, ht-fey-t,   wd-2*fex, t);
   // 2 vertical lines
   ssd13.drawRect (fex, fey,        t, ht-2*fey);
   ssd13.drawRect (fex+t, fey,      t, ht-2*fey);
   ssd13.drawRect (wd-fex-2*t, fey, t, ht-2*fey);
   ssd13.drawRect (wd-fex-t,   fey, t, ht-2*fey);

   ssd13.display ();
}

void Display::clear ()
{
   ssd13.clear ();
   ssd13.display ();
}

void Display::drawSymbol (Display::Symbol symbol)
{
   switch (symbol) {
     case UpArrow:   drawArrow (true,  countdownTime);     break;
     case DownArrow: drawArrow (false, countdownTime);     break;
     case Cross:     drawCross ();                         break;
     case Box:       drawBox ();                          break;
     case None:      clear ();                             break;
   }
}
