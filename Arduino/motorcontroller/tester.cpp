//
// tester.cpp -- tester for RiksLift
//
#define _DEBUG 1
#include "debug.h"

#include "mainapp.h"
#include "timer.h"
#include "liftglobals.h"

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

struct Command 
{
  char c;
  const char *name;
  void (*function) (bool, int);
};

static Command commands [] = {
  {'?', "Show this menu",                  showMenu},
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