#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <math.h>

using namespace std;

int writeAlarm(const string name, const string alarm);
int checkAlarm(const string alarm);
int checkRange(const string setting, char lower, char higher);
int checkDate(const string date, const string alarmTime);
string setAlarmSetting(const int option, const string alarm);
int stringToInt(string str);


int writeAlarm(const string name, const string alarm, const string setting, const string settingData) {
	//declare filename to be written to
	
	ofstream outfile; // declare the file object
	outfile.open(alarmFileName); // open the file
	if (!outfile.is_open()) {
		cerr << "Unable to open file" << endl;
		return -1; // Unable to open file
	}
	
	outfile << name << "," << alarm << "," << setting << "," << settingData << "\n";
	
	outfile.close();
	return 0;
}


int checkAlarm(const string alarm) {
	//check alarm for empty string
	if (alarm.empty()) {
		cerr << "Error: Empty string!" << endl;
		return -3;
	}
	
	//declare counters
	int count = 0;	//use counter for legibility
	int numHoursDigits = 0;
	int numMinutesDigits = 0;
	
	//check for  (weird bug involving cin operator)
	for (int i = 0; i < alarm.length(); i++) {
		if (alarm[i] == '') {
			cerr << "Don't use arrow keys!" << endl;
			return -3;
		}
	}
	
	//check for digits only before and after the colon
	while (alarm[count] != ':') {
		if (alarm[count] < '0' || alarm[count] > '9') {
			cerr << "Error: Digits only!" << endl;
			return -3;
		}
		count++;
		numHoursDigits++;
		if (numHoursDigits > 2) {
			cerr << "Error: Too many digits" << endl;
			return -3;
		}
	}
	count++;
	while (alarm[count]) {
		if (alarm[count] < '0' || alarm[count] > '9') {
			cerr << "Digits only!" << endl;
			return -3;
		}
		count++;
		numMinutesDigits++;
		if (numMinutesDigits > 2) {
			cerr << "Error: Too many digits" << endl;
			return -3;
		}
	}
	
	//valid time (0 <= hours <= 24, 0 <= minutes <= 59)
	if (numHoursDigits == 2) {
		if (alarm[0] < '0' || alarm[0] > '2') {
			cerr << "Error: Invalid time" << endl;
			return -3;
		}
		if (alarm[0] == '2' && alarm[1] > '3') {
			cerr << "Error: Invalid time" << endl;
			return -3;
		}
	}
	if (numMinutesDigits == 2) {
		if (alarm[numHoursDigits + 1] < '0' || alarm[numHoursDigits + 1] > '5') {
			cerr << "Error: Invalid time" << endl;
			return -3;
		}
	}
	
	//check for empty minutes or hours
	if (numHoursDigits == 0 || numMinutesDigits == 0) {
		cerr << "Error: Too few digits" << endl;
		return -3;
	}
	
	//whoa
	return 0;
}

int checkRange(const string setting, char lower, char higher) {
	//check setting for empty string
	if (setting.empty()) {
		cerr << "Error: Empty string!" << endl;
		return -4;
	}
	
	//check for  (weird bug involving cin operator)
	for (int i = 0; i < setting.length(); i++) {
		if (setting[i] == '') {
			cerr << "Don't use arrow keys!" << endl;
			return -2;
		}
	}
	
	//check to make sure a valid option was chosen
	if (setting.length() > 1) {
		cerr << "Error: Invalid option" << endl;
		return -4;
	}
	else if (setting[0] < lower || setting[0] > higher) {
		cerr << "Error: Invalid option" << endl;
		return -4;
	}
	
	return 0;
}

int checkDate(const string date, const string alarmTime) {//DD/MM/YYYY
	//check for empty string
	if (date.empty()) {
		cerr << "Error: Empty string!" << endl;
		return -5;
	}
	
	//declare counters
	int numDayDigits = 0;
	int numMonthDigits = 0;
	int numYearDigits = 0;
	bool isDay = true;
	bool isMonth = false;
	bool isYear = false;
	
	for (int i = 0; i < date.length(); i++) {
		//check for arrow keys
		if (date[i] == '') {
			cerr << "Don't use arrow keys!" << endl;
			return -5;
		}
		
		//check for format
		if ((date[i] < '0' || date[i] > '9') && date[i] != '/' && date[i] != 0) {
			cerr << "Error: Check your format" << endl;
			return -5;
		}
		
		//switch states of day, month, year according to position of i in the string date
		if (date[i] == '/') {
			if (isDay) {
				isDay = false;
				isMonth = true;
			}
			else {
				isMonth = false;
				isYear = true;
			}
		}
		else {
			//increment day/month/year counts
			if (isDay)
				numDayDigits++;
			else if (isMonth)
				numMonthDigits++;
			else
				numYearDigits++;
		}
	}
	
	//check for correct amount of day/month/year digits
	if (numDayDigits != 2) {
		cerr << "Error: Invalid day" << endl;
		return -5;
	}
	if (numMonthDigits != 2) {
		cerr << "Error: Invalid month" << endl;
		return -5;
	}
	if (numYearDigits != 4) {
		cerr << "Error: Invalid year" << endl;
		return -5;
	}
	
	//check for valid month and year
	if ((date[0] == '0' && date[1] == '0') || (date[0] == '3' && date[1] > '1') || date[0] > '3') {
		cerr << "Error: Invalid month" << endl;
		return -5;
	}
	if ((date[3] == '0' && date[4] == '0') || (date[3] == '1' && date[4] > '2') || date[3] > '1') {
		cerr << "Error: Invalid month" << endl;
		return -5;
	}
	if (date[6] == '0' && date[7] == '0' && date[8] == '0' && date[9] == '0') {
		cerr << "Error: Invalid year" << endl;
		return -5;
	}
	
	//initialize time and set to local to compare dates
	time_t now = time(0);
	tm* ltm = localtime(&now);
	
	//declare day, month, year, hour, minute ints
	const int day = stringToInt(date.substr(0,2));
	const int month = stringToInt(date.substr(3,2));
	const int year = stringToInt(date.substr(6,4));
	const int hour = stringToInt(alarmTime.substr(0,2));
	const int minute = stringToInt(alarmTime.substr(3,2));
	
	//cannot choose a date from the past
	if (year < (ltm->tm_year + 1900)) {
		cerr << "Choose a year in the future!" << endl;
		return -5;
	}
	if (month < (ltm->tm_mon + 1) && year == ltm->tm_year) {
		cerr << "Choose a month in the future!" << endl;
		return -5;
	}
	if (day < ltm->tm_mday && month == ltm->tm_mon) {
		cerr << "Choose a day in the future!" << endl;
		return -5;
	}
	if (hour < ltm->tm_hour && day == ltm->tm_mday) {
		cerr << "Choose a time in the future! (time)" << endl;
		return -5;
	}
	if (minute < ltm->tm_min && hour == ltm->tm_hour) {
		cerr << "Choose a time in the future! (time)" << endl;
		return -5;
	}
	
	//freak off???
	return 0;
}

int checkYesOrNo(const string yn) {
	//check for empty string
	if (yn.empty()) {
		cerr << "Error: Empty string!" << endl;
		return -6;
	}
	
	//check for  (weird bug involving cin operator)
	for (int i = 0; i < yn.length(); i++) {
		if (yn[i] == '') {
			cerr << "Don't use arrow keys!" << endl;
			return -6;
		}
	}
	
	//check for longer than 1 character
	if (yn.length() > 1) {
		cerr << "Error: Too long, only input one letter" << endl;
		return -6;
	}
	
	//check for only a y, n, Y, or N
	if (yn[0] != 'y' && yn[0] != 'n' && yn[0] != 'Y' && yn[0] != 'N') {
		cerr << "Error: Invalid character" << endl;
		return -6;
	}
	
	return 0;
}

string setAlarmSetting(const int option, const string alarm) {
	//initialize strings
	string daysOfWeek;
	string input;
	
	//different options will yield a different combination of 1's and 0's representing on and off respectively per that day
	if (option == 1)
		daysOfWeek = "1111111";
	else if (option == 2)
		daysOfWeek = "0111110";
	else if (option == 3)
		daysOfWeek = "1000001";
	else if (option == 4) {	//unique case for option: date
		string date;
		cout << "Enter a date (DD/MM/YYYY): ";
		getline(cin, date);
		while (checkDate(date, alarm)) {
			cerr << "Please enter a valid date (DD/MM/YYYY): ";
			getline(cin, date);
		}
		return date;	//will return a date with values separated by '/'
	}
	else {
		const int daysInWeek = 7;
		const string days[daysInWeek] = {"Sunday: ", "Monday: ", "Tuesday: ", "Wednesday: ", "Thursday: ", "Friday: ", "Saturday: "};
		cout << "Select the days you wish the alarm to sound." << endl;
		
		//loop will ask for y/n for each day of the week and then add a '1' or '0' to the string accordingly
		for (int i = 0; i < daysInWeek; i++) {
			//get the input
			cout << days[i];
			getline(cin, input);
			while (checkYesOrNo(input)) {
				cout << "Please enter 'y' or 'n': ";
				getline(cin, input);
			}
			//interpret the y or n as a 1 or 0
			if (input == "Y" || input == "y")
				daysOfWeek = daysOfWeek + "1";
			else
				daysOfWeek = daysOfWeek + "0";
		}
	}
	
	//if 1, 2, 3, or 5
	return daysOfWeek;
}

//conversion of string to int (no error checking, input should be error checked prior to using this function)
int stringToInt(const string str) {
	int num = 0;
	int count = 0;
	
	for (int i = str.length() - 1; i >= 0; i--) {
		num = num + (str[i] - '0') * pow(10, count);
		count++;
	}
	
	return num;
}

int main() {
	//declare variables
	string name;
	string alarm;
	string setting;
	
	//get name
	cout << "Enter a name: ";
	getline(cin, name);
	while (checkName(name)) {
		cerr << "Please enter a valid name: ";
		getline(cin, name);
	}
	
	//get alarm
	cout << "Enter your alarm time: ";
	getline(cin, alarm);
	while (checkAlarm(alarm)) {
		cerr << "Please enter a valid name: ";
		getline(cin, alarm);
	}
	
	//get setting
	cout << "Alarm setting:" << endl << "1) Everyday" << endl << "2) Weekdays" << endl << "3) Weekends" 
		<< endl << "4) Date" << endl << "5) Custom" << endl << "Enter your setting: ";
	getline(cin, setting);
	while (checkRange(setting)) {
		cerr << "Please enter a valid setting (1, 2, 3, 4, or 5): ";
		getline(cin, setting);
	}
	
	//implement setting
	const int option = setting[0] - '0';
	
	//write to file
	cout << (writeAlarm(name, alarm, setting, setAlarmSetting(option, alarm))) << endl;
	
	//NICE
	return 0;
}

