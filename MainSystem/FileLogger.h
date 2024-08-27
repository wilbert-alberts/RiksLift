// C++ program to implement a basic logging system. 

#include <fstream> 
#include <iostream>

#include "StreamLogger.h"

using namespace std; 

class FileLogger: public StreamLogger { 
public: 
	// Constructor: Opens the log file in append mode 
	FileLogger(const string& filename) : StreamLogger(logFile)
	{ 
		logFile.open(filename, ios::app); 
		if (!logFile.is_open()) { 
			std::cerr << "Error opening log file." << endl; 
		} 
	} 

	// Destructor: Closes the log file 
	~FileLogger() { logFile.close(); } 

private: 
	ofstream logFile; // File stream for the log file 
}; 