// C++ program to implement a basic logging system. 

#ifndef SCREENLOGGER_H
#define SCREENLOGGER_H


#include <iostream> 
#include "StreamLogger.h"
using namespace std; 

class ScreenLogger: public StreamLogger { 
public: 
	// Constructor: Opens the log file in append mode 
	ScreenLogger() : StreamLogger(std::cerr)
	{ 
	} 

	// Destructor: Closes the log file 
	~ScreenLogger() {  } 

private: 
}; 

#endif