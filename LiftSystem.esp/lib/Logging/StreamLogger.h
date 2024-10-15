// C++ program to implement a basic logging system. 

#ifndef STREAMLOGGER_H
#define STREAMLOGGER_H

#include <ctime> 
#include <ostream> 
#include <sstream> 

#include "BaseLogger.h"

// Enum to represent log levels 

class StreamLogger: public BaseLogger { 
public: 
	// Constructor: Opens the log file in append mode 
	StreamLogger(std::ostream& o) : BaseLogger(), os(o)
	{ 
	} 

	// Destructor: Closes the log file 
	~StreamLogger() {  } 

	virtual void logMessage(const string& msg)
	{
		// Output to stream 
		os << msg;
		os.flush();
	} 
private:
	std::ostream& os; // File stream for the log file 

}; 

#endif