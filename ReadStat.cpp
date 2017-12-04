#include "Log.hpp"
#include "ReadStat.hpp"
#include <fstream>
#include <iostream>

using namespace std;

Log ReadStat::logger;
ReadStat::ReadStat(string nameOfFile) {
	data = NULL;
	length = 0;
	filename = nameOfFile;
	min = -1;
	max = -1;
	mean = -1;
	median = -1;
	for (int i = 0; i < bucketsLen; i++) {
		buckets[i] = 0;
	}
}
void ReadStat::readData() {//reads standard stats for days of week
	//only read if file exists
	if (fileExists()) {
		//initalize variables
		string line = "";
		length = 0;
		ifstream statfile;
		statfile.open(filename);
		
		//calculate amount of datapoints in file
		while(!statfile.eof()){
			getline(statfile, line);
			if(!line.empty() && line.compare("\r")!= 0)
				length++;
		}
		statfile.close();
		line = "";

		//start reading again now that length is known
		ifstream statfileR;
		statfileR.open(filename);
		
		//create a new array for entries and assign to data
		int* tmp = data;
		data = new int[length];
		delete tmp;
		
		//populate the array
		for (int i = 0; i < length; i++) {
			getline(statfileR, line);
			data[i] = stoi(line);
		}
		statfileR.close();
	}
	else {
		logger.log("WARN", "Tried to read stat file" + filename + ", does not exist, ignored");
	}
}
void ReadStat::specialDataLen(const int newLen) {//used to calculate length of special datasets
	data = new int[newLen];
	length = 0;
}
void ReadStat::specialData(const int newLen, const int* dataset) {//calculates special datasets
	for (int i = length; i < newLen + length; i++) {
		data[i] = dataset[i - length];
	}
	length += newLen;
}
void ReadStat::setMax() {//calculate max value of dataset
	max = data[0];
	for (int i = 0; i < length; i++) {
		if (data[i] > max)
			max = data[i];
	}
}
void ReadStat::setMin() {//calculate min value of dataset
	min = data[0];
	
	for (int i = 0; i < length; i++) {
		if (data[i] < min)
			min = data[i];
	}
}
void ReadStat::setMean() {//calculate mean value of dataset
	mean = 0;
	sort();
	for (int i = 0; i < length; i++) {
		mean += data[i];
	}
	mean /= length;
}
void ReadStat::sort() {//sort dataset
	int temp;
	for (int i = 0; i < length; i++) {
		for (int j = 0; j < length - 1; j++) {
			if (data[j + 1] > data[j]) {
				temp = data[j];
				data[j] = data[j + 1];
				data[j + 1] = temp;
			}
		}
	}
}
void ReadStat::setMedian() {//calculate median value of dataset
	if (length % 2)
		median = data[length / 2];
	else
		median = ((data[length / 2 - 1] + data[length / 2]) / 2);
}
void ReadStat::histogram() {//calculate a histogram for dataset
	for (int i = 0; i < length; i++) {
		if (data[i] < 10)
			buckets[0]++;
		else if (data[i] < 30)
			buckets[1]++;
		else if (data[i] >= 300)
			buckets[7]++;
		else
			buckets[(data[i] / 60) + 2]++;
	}
}
bool ReadStat::fileExists() {//check if file exists
	ifstream statfile;
	statfile.open(filename);
	if(statfile.is_open()) {
		statfile.close();
		return true;	//file exists
	}
	statfile.close();
	return false; //file does not exist (yet) :smirk:
}
string* ReadStat::getStats() {//process all statistics
	string* printStats;
	//do if there is data
	if (length > 0) {
		int daysInWeek = 7;
		string daysOfWeek[7];
		
		setMax();
		setMin();
		setMean();
		setMedian();
		histogram();
		
		//add all printing to a string array to allow for good formatting between program and summary file
		printStats = new string[13];
		printStats[0] = "Max: " + to_string(max) + "\r\n";
		printStats[1] = "Min: " + to_string(min) + "\r\n";
		printStats[2] = "Mean: " + to_string(mean) + "\r\n";
		printStats[3] = "Median: " + to_string(median) + "\r\n\r\n";
		printStats[4] = "Histogram:\r\n";
		printStats[5] = "[0,9] sec: " + to_string(buckets[0]) + "\r\n";
		printStats[6] = "[10,29] sec: " + to_string(buckets[1]) + "\r\n";
		printStats[7] = "[30,59] sec: " + to_string(buckets[2])	 + "\r\n";
		printStats[8] = "[1,2) min: " + to_string(buckets[3]) + "\r\n";
		printStats[9] = "[2,3) min: " + to_string(buckets[4]) + "\r\n";
		printStats[10] = "[3,4) min: " + to_string(buckets[5]) + "\r\n";
		printStats[11] = "[4,5) min: " + to_string(buckets[6]) + "\r\n";
		printStats[12] = "[5-10] min: " + to_string(buckets[7]) + "\r\n";					  
	}
	else {
		printStats = new string[1];
		printStats[0] = "There is no data available for this period\r\n";
	}
	return printStats;
}
void ReadStat::displayStats() {//display stats
	string* printStats = getStats();
	if (length > 0) {
		cout << endl << endl;
		for (int i = 0; i < 13; i++) {
			cout << "\t" << printStats[i];
		}
	}
	else {
		cout << "\t" << printStats[0];
		
	}
	//delete printStats;
}
void ReadStat::writeStats(const string* printStats, const string title) {//write stats to summary file
	ofstream writefile;
	
	//write sunday first with format, at this time overwrite whichever file previously existed or make one if it didn't
	if (!title.compare("SUNDAY")) {
		writefile.open("Statistics Summary.stat");
		writefile << "STATISTICS SUMMARY\r\n__________________\r\n";
	}
	//otherwise, append
	else
		writefile.open("Statistics Summary.stat", ios::app | ios:: out);
	
	//write stats with formating
	writefile << "\r\n" << title << ":\r\n";
	if (length > 0) {
		for (int i = 0; i < 13; i++) {
			writefile << printStats[i] << endl;
		}
	}
	//write no data if no data
	else
		writefile << printStats[0] << endl;
	writefile.close();
}
int ReadStat::getLength() {//get amount of data points
	return length;
}
int* ReadStat::getData() {//get data in an int array
	return data;
}
void ReadStat::setLogger(Log log) {//logger
	ReadStat::logger = log;
}
