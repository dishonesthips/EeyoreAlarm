#include <iostream>
#include <fstream>
#include <string>
#include <time.h>

using namespace std;

enum SEVERITY {TRACE, DEBUG, INFO, WARNING, FATAL};

int log(const string source, const string message, const SEVERITY severity) {
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
	
	logfile << asctime(ltm) << "\r\n" << "Source: " << source << "\r\n" << message << "\r\n" << severity << "\r\n\r\n";
	logfile.close();
	
	return 0;
}

int main() {
	log("hello.exe", "REEEE", FATAL);
}