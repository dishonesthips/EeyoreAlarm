#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <unistd.h>
using namespace std;

class Log {
	public:
		Log();
		void log(string, string);
		
	private:
		string filename;
		string message;
		string severity;
};

Log::Log() {
	//initially set strings to empty strings
	message = "";
	severity = "";
	-
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

void Log::log(string a, string b) {
	ofstream logfile; // declare the file object
	logfile.open(filename, ios::app | ios::out); // open the file
	
	//initialize time and set to local
	time_t now = time(0);
	tm* ltm = localtime(&now);
	
	//set message and severity to be written
	message = a;
	severity = b;
	
	//write data ato log
	logfile << (ltm->tm_year + 1900) << "-" << (ltm->tm_mon + 1) << "-" << ltm->tm_mday << "\t" 
			<< ltm->tm_hour << ":" << ltm->tm_min << ":" << ltm->tm_sec << "\t\t(" << severity << "):\t " << message << "\r\n";
	logfile.close();
}

int main() {
	Log mylog;
	
	mylog.log("stoppp","FATAL");
	mylog.log("brooo","WARNING");
	mylog.log("asdfd","INFO");
	
	sleep(1);
	Log log2;
	log2.log("yeoww","INFO");
}