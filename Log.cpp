#include "Log.hpp"
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

//Log member function declarations
Log::Log(){//constructor opens log file named with the time that the program was run
	//initialize time and set to local
	time_t now = time(0);
	tm* ltm = localtime(&now);
	
	//declare filename to be written to
	filename = to_string(ltm->tm_year + 1900) + "-" + to_string(ltm->tm_mday) + "-" + to_string(ltm->tm_mon + 1) + "-" +
							to_string(ltm->tm_hour) + "-" + to_string(ltm->tm_min) + "-" + to_string(ltm->tm_sec) + ".log";
							
	ofstream logfile; // declare the file object
	logfile.open(filename, ios::app | ios::out); // open the file
	logfile.close();
}

void Log::log(string severity, string message){ //logs a given message
	ofstream logfile; // declare the file object
	logfile.open(filename, ios::app | ios::out); // open the file
	
	//initialize time and set to local
	time_t now = time(0);
	tm* ltm = localtime(&now);
	
	//write data to log
	logfile << (ltm->tm_year + 1900) << "-" 
            << setfill('0') << setw(2) << (ltm->tm_mon + 1) << "-" 
            << setfill('0') << setw(2) << ltm->tm_mday << "\t" 
            << setfill('0') << setw(2) << ltm->tm_hour << ":" 
            << setfill('0') << setw(2) << ltm->tm_min << ":" 
            << setfill('0') << setw(2) << ltm->tm_sec << "\t(" 
            << severity << "):\t " 
            << message << "\r\n";
	logfile.close();
}