#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

using namespace std;

int main() {
	//declare variables
	const int NUM_SECONDS = 1;
	const int EST = -5;
	const int testLimit = 10;
	const int sleepTime = 1;
	
	//initialize time and set to local
	time_t now = time(0);
	tm* ltm = localtime(&now);
	
	//print date (IDK WHY 1900)
	cout << "Year: " << 1900 + ltm->tm_year<<endl;
	cout << "Month: "<< 1 + ltm->tm_mon<< endl;
	cout << "Day: "<<  ltm->tm_mday << endl;
	cout << "Days since Sunday: " << ltm->tm_wday << endl;
	
	//print the local time once per second
	for (int i = 0; i < testLimit; i++) {
		now = time(0);
		tm* ltm = localtime(&now);
		sleep(sleepTime);
		cout << "Current time: " << (ltm->tm_hour) << ":" << (ltm->tm_min) << ":" << (ltm->tm_sec) << endl;
	}
	
	//yah eh
	return 0;
}