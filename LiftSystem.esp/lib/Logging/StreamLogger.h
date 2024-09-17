// // C++ program to implement a basic logging system. 

#ifndef STREAMLOGGER_H
#define  STREAMLOGGER_H

// #include <ctime> 
// #include <ostream> 
// #include <sstream> 
// using namespace std; 

// // Enum to represent log levels 
// enum LogLevel { DEBUG, INFO, WARNING, ERROR, CRITICAL }; 

// class StreamLogger { 
// public: 
// 	// Constructor: Opens the log file in append mode 
// 	StreamLogger(std::ostream& o) : os(o)
// 	{ 
// 	} 

// 	// Destructor: Closes the log file 
// 	~StreamLogger() {  } 

// 	// Logs a message with a given log level 
// 	void log(LogLevel level, const string& message) 
// 	{ 
// 		// Get current timestamp 
// 		time_t now = time(0); 
// 		tm* timeinfo = localtime(&now); 
// 		char timestamp[20]; 
// 		strftime(timestamp, sizeof(timestamp), 
// 				"%Y-%m-%d %H:%M:%S", timeinfo); 

// 		// Create log entry 
// 		ostringstream logEntry; 
// 		logEntry << "[" << timestamp << "] "
// 				<< levelToString(level) << ": " << message 
// 				<< endl; 

// 		// Output to stream 
// 		os << logEntry.str(); 
// 		os.flush();

// 	} 

// private: 
// 	std::ostream& os; // File stream for the log file 

// 	// Converts log level to a string for output 
// 	string levelToString(LogLevel level) 
// 	{ 
// 		switch (level) { 
// 		case DEBUG: 
// 			return "DEBUG"; 
// 		case INFO: 
// 			return "INFO"; 
// 		case WARNING: 
// 			return "WARNING"; 
// 		case ERROR: 
// 			return "ERROR"; 
// 		case CRITICAL: 
// 			return "CRITICAL"; 
// 		default: 
// 			return "UNKNOWN"; 
// 		} 
// 	} 
// }; 

#endif