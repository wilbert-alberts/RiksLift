//
// tester.cpp -- tester for RiksLift
//
#define _DEBUG 0
#include "debug.h"

#include "mainapp.h"
#include "timer.h"
#include "liftglobals.h"
#include "pushbutton.h"
#include "buzzer.h"
#include "display.h"
#include "identity.h"

#include "tester.h"

Tester tester;

#if _DEBUG==1
static void menu ();
static void showMenu (bool b, int num);
#endif

void Tester::setup () 
{
#if _DEBUG==1
   showMenu (false, 0);
#endif
}

void Tester::loop () 
{
#if _DEBUG==1
   menu ();
#endif
}

#if _DEBUG==1




static void testButton ();
static void testEMO ();
static void testScreen ();
static void testIdentity ();

void ButtonDownStatusCB (String payload, void *context)
{
   DEBUG (">< ButtonDownStatusCB (payload = %s)\n", payload.c_str ());
}


static void testClear (bool b, int num)
{
   pushbutton.setLED (LitPushbutton::None, false);
}

static void testRed (bool b, int num)
{
   pushbutton.setLED (LitPushbutton::Red, b);  
}

static void testGreen (bool b, int num)
{
   pushbutton.setLED (LitPushbutton::Green, b);
}

static void testBlue (bool b, int num)
{
   pushbutton.setLED (LitPushbutton::Blue, b);
}

static void testLiftPosition (bool b, int num)
{
    LiftState l = LiftState (num % 6);
    mainApp.setState (l);
}

static void testAlarm (bool b, int num)
{
   pushbutton.alarm (b);
}

static void testBuzzer (bool b, int num)
{
   buzzer.buzz (b); 
}

static void testScreen (bool b, int num)
{
   DEBUG ("Simulate touch screen %s\n", toCCP (b));
   screen.setSimPushed (b);
}


struct Command 
{
  char c;
  const char *name;
  void (*function) (bool, int);
};

static Command commands [] = {
  {'?', "Show this menu",                  showMenu},
  {'a', "Alarm pushbutton",                testAlarm},
  {'b', "Blue",                            testBlue},
  {'c', "Clear LEDs",                      testClear},
  {'g', "Green",                           testGreen},
  {'p', "set lift Position",               testLiftPosition},
  {'r', "Red",                             testRed},
  {'t', "sim Touch screen",                testScreen},
  {'z', "BuZZ",                            testBuzzer}
};

const int nCommands = sizeof(commands)/sizeof (Command);

static void showMenu (bool b, int num)
{
   DEBUG ("\n");
   for (int i = 0; i < nCommands; i++) {
      DEBUG (" %c %s\n", commands[i].c, commands[i].name);
   }
   DEBUG ("\n");
}

static void menu ()
{
   static int num = 0;

   while (Serial.available () > 0) {
      bool negative = false;
      int c = Serial.read ();
      if (isdigit (c)) num = 10 * num + (c-'0');
      else if (c == '-') negative = !negative;
      else {
         if (negative) num = -num;
         bool isu = isupper (c);
         for (int i = 0; i < nCommands; i++) {
            if (tolower (c) == commands [i].c) {
               DEBUG (">  %s %s\n", commands [i].name, isu?"true":"false");
               commands [i].function (isu, num);
               DEBUG ("<  %s\n\n", commands [i].name);
               num = 0;
               break;
            }
         }
      }
   }   
}

#endif