#include "Log.hpp"
#include <string>
#include <iostream>

using namespace std;

Log ReadStatList::logger;

ReadStatList::ReadStatList() {//constructor
	string num = "";
	//populate array of stats, 0-6 are normal, 7-9 are special
	for (int i = 0; i < length; i++) {
		num = to_string(i);
		stats[i] = new ReadStat("stats" + num + ".txt");
	}

}


void ReadStatList::setLogger(Log log){//logger
	ReadStatList::logger = log;
}


int ReadStatList::runStats() {//run stats
	//simple read for the days of the week stats
	for (int i = 0; i < lengthDays; i++) {
		stats[i]->readData();
	}
	
	
	//unique reads for the special statistics
	//first find the total length of the array
	int tmpLen = 0;
	for (int i = 1; i < lengthDays - 1; i++) {
		tmpLen += stats[i]->getLength();
	}
	stats[7]->specialDataLen(tmpLen);
	//then pass in the desired values to be summed
	for (int i = 1; i < lengthDays - 1; i++) {
		stats[7]->specialData(stats[i]->getLength(), stats[i]->getData());
	}
	tmpLen = 0;
	//do this for all three special functions, but using the desired values
	//i.e. weekday stats only use weekday values, weekend only weekend, total uses all
	
	tmpLen += stats[0]->getLength();
	tmpLen += stats[lengthDays - 1]->getLength();
	stats[8]->specialDataLen(tmpLen);
	stats[8]->specialData(stats[0]->getLength(), stats[0]->getData());
	stats[8]->specialData(stats[lengthDays - 1]->getLength(), stats[lengthDays - 1]->getData());
	tmpLen = 0;
	
	for (int i = 0; i < lengthDays; i++) {
		tmpLen += stats[i]->getLength();
	}
	stats[9]->specialDataLen(tmpLen);
	for (int i = 0; i < lengthDays; i++) {
		stats[9]->specialData(stats[i]->getLength(), stats[i]->getData());
	}
	
	//make an array of day name for formatting
	string titles[10] = {"SUNDAY", "MONDAY", "TUESDAY", "WEDNESDAY", "THURSDAY", "FRIDAY", "SATURDAY", "WEEKDAYS", "WEEKENDS", "TOTAL"};
	
	//ask user which stats to view
	cout << "\n\n\tList of Statistics " << endl <<
			"\t1. Sunday" << endl <<
			"\t2. Monday" << endl <<
			"\t3. Tuesday" << endl <<
			"\t4. Wednesday" << endl <<
			"\t5. Thursday" << endl <<
			"\t6. Friday" << endl <<
			"\t7. Saturday" << endl <<
			"\t8. Weekdays" << endl <<
			"\t9. Weekends" << endl <<
			"\t10. Total" << endl <<
			"\tWhich statistic would you like to view? (1-10): ";
	string statNum;
	getline(cin, statNum);
	while (AlarmList::checkRange(statNum, 1, length)) {
		cout << "\tPlease enter a single digit in range [1,10]: ";
		getline(cin, statNum);
	}
	
	//display the desired statistics
	stats[stoi(statNum) - 1]->displayStats();
	
	//write all statistics to summary file
	stats[0]->writeStats(stats[0]->getStats(), titles[0]);
	for (int i = 1; i < 10; i++) {
		stats[i]->writeStats(stats[i]->getStats(), titles[i]);
	}
	logger.log("INFO", "Writing and printing stats success");
	return 0;
}

