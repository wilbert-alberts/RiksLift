// // C++ program to implement a basic logging system. 

#ifndef BASELOGGER_H
#define BASELOGGER_H

#include <ctime> 
#include <sstream> 

// Enum to represent log levels 
enum LogLevel { DEBUG, INFO, WARNING, ERROR, CRITICAL }; 

class BaseLogger { 
public: 
	// Constructor: Opens the log file in append mode 
	BaseLogger()
	{ 
	} 

	// Destructor: Closes the log file 
	~BaseLogger() {  } 

	// Logs a message with a given log level 
	void log(LogLevel level, const std::string& message) 
	{ 
		// Get current timestamp 
		time_t now = time(0); 
		tm* timeinfo = localtime(&now); 
		char timestamp[20]; 
		strftime(timestamp, sizeof(timestamp), 
				"%Y-%m-%d %H:%M:%S", timeinfo); 

		// Create log entry 
		std::ostringstream logEntry; 
		logEntry << "[" << timestamp << "] "
				<< levelToString(level) << ": " << message 
				<< std::endl; 

		// Output to stream 
		logMessage(logEntry.str());
	} 


protected: 
	virtual void logMessage(const std::string& msg) = 0;

	// Converts log level to a string for output 
	std::string levelToString(LogLevel level) 
	{ 
		switch (level) { 
		case DEBUG: 
			return "DEBUG"; 
		case INFO: 
			return "INFO"; 
		case WARNING: 
			return "WARNING"; 
		case ERROR: 
			return "ERROR"; 
		case CRITICAL: 
			return "CRITICAL"; 
		default: 
			return "UNKNOWN"; 
		} 
	} 
}; 

#endif