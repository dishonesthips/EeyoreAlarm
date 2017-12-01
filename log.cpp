#include <iostream>
#include <fstream>
#include <string>
#include <time.h>

using namespace std;

int log(const string message, const string severity) {
	//initialize time and set to local
	time_t now = time(0);
	tm* ltm = localtime(&now);
	
	
	//declare filename to be written to
	const string filename = to_string(ltm->tm_year + 1900) + "-" + to_string(ltm->tm_mday) + "-" + to_string(ltm->tm_mon + 1) + "-" +
							to_string(ltm->tm_hour) + "-" + to_string(ltm->tm_min) + "-" + to_string(ltm->tm_sec) + ".log";
	
	ofstream logfile; // declare the file object
	logfile.open(filename, ios::app | ios::out); // open the file
	if (!logfile.is_open()) {
		cerr << "Unable to open file" << endl;
		return -1; // Unable to open file
	}
	
	logfile << (ltm->tm_year + 1900) << "-" << (ltm->tm_mon + 1) << "-" << ltm->tm_mday << "\t" 
			<< ltm->tm_hour << ":" << ltm->tm_min << ":" << ltm->tm_sec << "\t\t(" << severity << "):\t " << message << "\r\n";
	logfile.close();
	
	return 0;
}

int main() {
	log("REEEE", "FATAL");
	log("REEEE", "WARNING");
}