//
// debug.h - a simple debugger
//
// This debugger uses Serial.printf to debug its output
// Note that if _Debug is 0, the arguments of DEBUG are NOT evaluated.
//   e.g. DEBUG ("The value of i = %d\n", i++);
//   If _DEBUG is defined as 0, i is NOT increased.
//
// BSla, 10 october 2023
//

#include <Arduino.h>
#include <stdarg.h>
#include <stdio.h>

static bool stopOutput = false; 

void DebugReal (const char *fmt, ...)
{
   if (!stopOutput) {
      va_list args;
      char buffer [200];
      va_start (args, fmt);
      vsnprintf (buffer, 200, fmt, args);
      va_end (args);
      Serial.print (buffer);
   }
   if (Serial.available ()) {
      char c = Serial.read ();
      if (c == 'x') {
        Serial.printf ("DEBUG output = %s\n", stopOutput?"off":"on");
        stopOutput = !stopOutput;
      }
   }
}


const char* toCCP (bool b) 
{
    static const char *True  = "true";
    static const char *False = "false";
    return b?True:False;
}