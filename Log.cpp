#include "Log.hpp"
#include <fstream>

using namespace std;

//Log member function declarations
Log::Log(){
	//initialize time and set to local
	time_t now = time(0);
	tm* ltm = localtime(&now);
	
	//declare filename to be written to
	filename = to_string(ltm->tm_year + 1900) + "-" +
		to_string(ltm->tm_mday) + "-" +
		to_string(ltm->tm_mon + 1) + "-" +
		to_string(ltm->tm_hour) + "-" +
		to_string(ltm->tm_min) + "-" +
		to_string(ltm->tm_sec) + ".log";
							
	ofstream logfile; // declare the file object
	logfile.open(filename, ios::app | ios::out); // open the file
	logfile.close();
}

void Log::log(string a, string b) {
	ofstream logfile; // declare the file object
	logfile.open(filename, ios::app | ios::out); // open the file
	
	//initialize time and set to local
	time_t now = time(0);
	tm* ltm = localtime(&now);

	//write data to log
	logfile << (ltm->tm_year + 1900) << "-" <<
		(ltm->tm_mon + 1) << "-" <<
		ltm->tm_mday << "\t" <<
		ltm->tm_hour << ":" <<
		ltm->tm_min << ":" <<
		ltm->tm_sec << "\t(" << a << "):\t " << b << "\r\n";
	logfile.close();
}
