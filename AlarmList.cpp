#include "Log.hpp"
#include "AlarmList.hpp"
#include <string>
#include <iostream>
#include <fstream>


using namespace std;

Log AlarmList::logger;

AlarmList::AlarmList(){//constructor
	alarms = NULL;
	int length = -1;
	
	//setting up (requesting pins) logging errors is within these functions
	int exitRet = gpioSetup(EXIT_PIN, rqExit, 0);
	int trigRet = gpioSetup(TRIGGER_PIN, rqTrigger, 0);
	int buzzerRet = gpioSetup(BUZZER_PIN, rqBuzzer, 1);
	
	if ((exitRet + trigRet + buzzerRet) != 0){
		logger.log("FATL","Error with pin setup");
	}
	
	gpio_set_value(BUZZER_PIN, 0);

	
}


int AlarmList::checkRange(const string setting, const int lower, const int higher) {//makes sure input falls within an integer range
	//check setting for empty string
	if (setting.empty()) {
		logger.log("WARN", "Given empty string for range, request try again");
		return -4;
	}
	
	for (int i = 0; i < setting.length(); i++) {
		if (setting[i]<'0' || setting[i]>'9') {
		logger.log("WARN", "Given input other than numbers, request try again");
			return -2;
		}
	}
	
	if (stoi(setting)< lower || stoi(setting) > higher) {
		logger.log("WARN", "Given an invalid option number, request try again");
		return -4;
	}
	
	return 0;
}
int AlarmList::checkYesOrNo(const string yn){//error checks yes or no input
	//check for empty string
	if (yn.empty()) {
		logger.log("WARN", "Given empty string, request try again");
		return -6;
	}
	
	//check for  (weird bug involving cin operator)
	for (int i = 0; i < yn.length(); i++) {
		if (yn[i] == '') {
			logger.log("WARN", "Given arrow key input for yes or no, request try again");
			return -2;
		}
	}
	
	//check for longer than 1 character
	if (yn.length() > 1) {
		logger.log("WARN", "Invalid input for yes or no, request try again");
		return -6;
	}
	
	//check for only a y, n, Y, or N
	if (yn[0] != 'y' && yn[0] != 'n' && yn[0] != 'Y' && yn[0] != 'N') {
		logger.log("WARN", "Invalid input for yes or no, request try again");
		return -6;
	}
	
	logger.log("INFO", "Y or N is valid");
	return 0;
}
void AlarmList::setLogger(Log log){ //assigns static logger member
	AlarmList::logger = log;
}


void AlarmList::runAlarm(){//runs in a loop handling the alarm system and alarms themselves
	
	int exitVal;
	int triggerVal;

	bool exitButtonHit= false;
	const int sleepTime = 1;
	time_t now = time(0);
	tm* ltm = localtime(&now);

	int buzzerSum;
	
	
	while(!exitButtonHit){
		buzzerSum = 0;
		now = time(0);
		tm* ltm = localtime(&now);
		
		sleep(sleepTime);
		cout << "\tCurrent time: " << (ltm->tm_hour) << ":" << (ltm->tm_min) << ":" << (ltm->tm_sec) << endl;
		
		if (ltm->tm_min%10 ==0 && ltm->tm_sec==0)//log every 10 minutes
			logger.log("TRCE","Running alarm system");
		
		exitVal = gpio_get_value(EXIT_PIN);
		triggerVal = gpio_get_value(TRIGGER_PIN);
		

		if(exitVal){
			logger.log("INFO","Manual request to stop running alarm system.");
			for (int i = 0; i < length; i++){
				alarms[i].resetAlarm(); //read motionstate
			}
			exitButtonHit = true;
			
		}
		
		for (int i = 0; i < length; i++){
			buzzerSum += alarms[i].tick(ltm, triggerVal); //read motionstate
		}
		
		if (buzzerSum){
			gpio_set_value(BUZZER_PIN, 1);
		}
		else	
			gpio_set_value(BUZZER_PIN, 0);
		
		
		
		
	}
	//calculate the number of alarms that will never ring again
	//only applies for oneTime alarms set to go on a specific date
	int numToDelete = 0;
	for (int i = 0; i < length; i++){
		if(alarms[i].getOneTime() && checkDate(alarms[i].getAlarmSchedule(), alarms[i].getFormatTime()) != 0)
			numToDelete++;
	}
	
	while(numToDelete){
		int pos = -1;
		//find position to delete
		for (int i = 0; i < length; i++){
			if(alarms[i].getOneTime() && checkDate(alarms[i].getAlarmSchedule(), alarms[i].getFormatTime()) != 0)
				pos = i;
		}
		
		delAlarm(pos);
		numToDelete--;
		
	}
	
	
	
	gpio_set_value(BUZZER_PIN, 0);
	
	gpioRelease(EXIT_PIN, rqExit);
	gpioRelease(TRIGGER_PIN, rqTrigger);
	gpioRelease(BUZZER_PIN, rqBuzzer);
	
}
int AlarmList::addAlarm(){ //gets input for appending alarm and does it on the fly
	
	string name;
	string alarm;
	string setting;
	
	//get name
	cout <<"\tEnter a name for your alarm: ";
	getline(cin, name);
	while (AlarmList::checkName(name)) {
		cerr << "\tPlease enter a valid name: ";
		getline(cin, name);
	}
	
	//get alarm
	cout << "\tEnter your alarm time using the 24 hour standard (hh:mm): ";
	getline(cin, alarm);
	while (AlarmList::checkAlarm(alarm)) {
		cerr << "\tPlease enter a valid time: ";
		getline(cin, alarm);
	}
	
	//get setting
	cout<< "\n\tAlarm setting:\n\t"
		<< "1. Everyday\n\t" 
		<< "2. Weekdays\n\t"
		<< "3. Weekends\n\t" 
		<< "4. Single Date\n\t"
		<< "5. Custom Schedule\n\n\t" 
		<< "Enter your setting: ";
	getline(cin, setting);
	while (checkRange(setting,1,5)) {
		cout << "Please enter a single digit in range [1,5]: ";
		getline(cin, setting);
	}
	
	//implement setting
	const int option = setting[0] - '0';
	Alarm* newAlarms = new Alarm[length + 1];	
	
	for (int i = 0; i < length; i++){
		newAlarms[i].setAlarmName(alarms[i].getAlarmName());
		newAlarms[i].setAlarmTime(alarms[i].getAlarmTime());
		newAlarms[i].setAlarmSchedule(alarms[i].getAlarmSchedule());
	}
	newAlarms[length].setAlarmName(name);
	newAlarms[length].setAlarmTime(stoi(alarm.substr(0,2)) * 60 + stoi(alarm.substr(3,2)));
	newAlarms[length].setAlarmSchedule(AlarmList::setAlarmSetting(option, alarm));
	length++;
	
	//rewrite alarms
	alarms = newAlarms;
	writeList();
	
	logger.log("INFO", "Alarm added successfully");
	return 0;
}
int AlarmList::delAlarm(){ //remove an alarm from the list of alarms
	if (length){
		string posStr;
		cout << "\tSelect which alarm to delete (number): ";
		getline(cin, posStr);
		while (checkRange(posStr, 1, length)) {
			cerr << "\tPlease enter a single digit in range [1," << length << "]: ";
			getline(cin, posStr);
		}
		int pos = stoi(posStr)-1;
		delAlarm(pos);
	}
	else{
		string s;
		cout<<"\n\tHit enter to continue... ";
		getline(cin, s);
		logger.log("WARN", "Tried to delete an alarm, no alarms to be deleted, ignored request");
	}
	
	return 0;
}
void AlarmList::displayList(){ // display list of alarms, user-friendly
	
	if (length){
		cout<<"\n\tThe following are your alarm(s):\n";
		for (int i = 0; i < length; i++) {
			cout << "\tALARM " << i+1 << "\n" << alarms[i].displayAlarm() << endl;
		}
	}
	else
		cout<<"\n\tIt seems like you have no alarms at the moment...\n";
}


int AlarmList::readList(){ //creates list of alarms from text file. only to be run once at start of program
	string line;
	length = 0;
	
	//opening file to get number of lines
	ifstream infile;            // declare the file object
	infile.open(filename);      // open the file
	
	//check the file exists
	if (!infile.is_open()){
		logger.log("WARN", "Cannot find alarm file, will be written later");
		return -1;
	}
	
	//find length of the array = num of alarms
	while(!infile.eof()){
		getline(infile,line);
		if(!line.empty() && line.compare("\r")!=0)
			length++;
	}
	infile.close();
	
	//create a new pointer for alarms
	Alarm* tmp = alarms;
	alarms = new Alarm[length];
	delete tmp;
	
	//open the file again, this time to be read
	ifstream infileR;
	infileR.open(filename);
	
	//declare counting variables
	string aName;
	string aTimeStr;
	string aSched;
	int index = 0;
	int charIndex;
	
	//get data from file
	while(!infileR.eof()){
		getline(infileR,line);
		if(!line.empty() && line.compare("\r")!=0){
			//reset all counting variables
			aName = "";
			aTimeStr = "";
			aSched = "";
			charIndex = 0;
			
			//append to name
			while (line[charIndex] != ',') {
				aName+= line[charIndex];
				charIndex++;
			}
			charIndex++;
			
			//append to time string, then convert to int
			while (line[charIndex] != ',') {
				aTimeStr += line[charIndex];
				charIndex++;
			}
			int aTime = stoi(aTimeStr);
			charIndex++;
			
			//append to schedule
			while (line[charIndex]) {
				aSched += line[charIndex];
				charIndex++;
			}
			charIndex++;
			
			//set alarm variables
			alarms[index].setAlarmName(aName);
			alarms[index].setAlarmTime(aTime);
			alarms[index].setAlarmSchedule(aSched);
			
			//increment index to go to next alarm
			index++;
		}
	}
	infileR.close();
	
	logger.log("INFO", "Alarm file read successfully");
	return 0;
}
int AlarmList::writeList(){ //appends an alarm to the file of alarms
	//only write if there are alarms to be written
	if (length > 0) {
		//open the file and write the variables to a line, separated by comma, for the first 
		ofstream alarmFirst;
		alarmFirst.open(filename);
		alarmFirst << alarms[0].getAlarmName() << "," << alarms[0].getAlarmTime() << "," << alarms[0].getAlarmSchedule() << "\r\n";
		alarmFirst.close();
		
		if (length > 1) {
			ofstream alarmFile;
			alarmFile.open(filename, ios::app | ios::out);

			for (int i = 1; i < length; i++)
				alarmFile << alarms[i].printAlarm()<< "\r\n";
			alarmFile.close();
		}
	}
	else {
		logger.log("WARN", "Prompted to write file, no alarms to write");
		return -1;
	}
	
	logger.log("INFO", "Alarm file written successfully");
	return 0;
}


bool AlarmList::isLeapYear(const int year){ //returns true if given year is leap year
	if (year % 400 == 0)
		return true;
	else if (year % 100 == 0)
		return false;
	else if (year % 4 == 0)
		return true;
	return false;
}
int AlarmList::checkName(const string input){//error checks for empty string and issues with cin
	//check for empty string
	if (input.empty()) {
		logger.log("WARN", "Given empty string, request try again");
		return -2;
	}
	
	//check for  (weird bug involving cin operator)
	for (int i = 0; i < input.length(); i++) {
		if (input[i] == '') {
			logger.log("WARN", "Given arrow key input for alarm name, request try again");
			return -2;
		}
	}
	
	//valid name
	return 0;
}
int AlarmList::checkAlarm(const string alarm){//error checks for HH:MM format 
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
	if (numHoursDigits != 2 || numMinutesDigits != 2) {
		cerr << "Error: Invalid time" << endl;
		return -3;
	}
	
	if (alarm[0] < '0' || alarm[0] > '2') {
		cerr << "Error: Invalid time" << endl;
		return -3;
	}
	if (alarm[0] == '2' && alarm[1] > '3') {
		cerr << "Error: Invalid time" << endl;
		return -3;
	}
	if (alarm[3] < '0' || alarm[3] > '5') {
		cerr << "Error: Invalid time" << endl;
		return -3;
	}
	
	//whoa
	return 0;
}
int AlarmList::checkDate(const string date, const string alarm){//non zero if given date and time have passed
	//check for empty string
	if (date.empty()) {
		logger.log("WARN", "Given empty string for date, request try again");
		return -5;
	}
	
	//check for  (weird bug involving cin operator)
	for (int i = 0; i < date.length(); i++) {
		if (date[i] == '') {
			logger.log("WARN", "Given arrow key input for date, request try again");
			return -2;
		}
	}
	
	//check for format
	if (date.length() != 10 || date[2] != '/' || date[5] > '/') {
		logger.log("WARN", "Given invalid format for date, request try again");
		return -5;
	}
	for (int i = 0; i < date.length(); i++) {
		if ((date[i] < '0' || date[i] > '9') && date[i] != '/') {
			logger.log("WARN", "Given invalid format for date, request try again");
			return -5;
		}
	}
	
	//declare day, month, year, hour, minute ints
	const int day = stoi(date.substr(0,2));
	const int month = stoi(date.substr(3,2));
	const int year = stoi(date.substr(6,4));
	const int hour = stoi(alarm.substr(0,2));
	const int minute = stoi(alarm.substr(3,2));
	
	//check for valid time and year
	const int numMonths = 12;
	const int daysInMonths[numMonths] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	
	//check for zeroes
	if (day == 0 || month == 0 || year == 0) {
		logger.log("WARN", "Given invalid date, request try again");
		return -5;
	}
	//check for bad month
	if (month > 12) {
		logger.log("WARN", "Given invalid date, request try again");
		return -5;
	}
	//check leap year as an outlier
	else if (AlarmList::isLeapYear(year) && month == 2 && day > 29) {
		logger.log("INFO", "Year is leap year");
		logger.log("WARN", "Given invalid date, request try again");
		return -5;
	}
	else if (!AlarmList::isLeapYear(year) && month == 2 && day > 28) {
		logger.log("INFO", "Year is leap year");
		logger.log("WARN", "Given invalid date, request try again");
		return -5;
	}
	//check each day for validitiy with respect to month
	for (int i = 0; i < numMonths; i++) {
		if (month == (i + 1) && day > daysInMonths[i]) {
		logger.log("WARN", "Given invalid date, request try again");
			return -5;
		}
	}
	
	//initialize time and set to local to compare dates
	time_t now = time(0);
	tm* ltm = localtime(&now);
	
	//cannot choose a date from the past
	if (year < (ltm->tm_year + 1900)) {
		logger.log("WARN", "Given date has passed, request try again");
		return -5;
	}
	else if (year == (ltm->tm_year + 1900)) {
		if (month < (ltm->tm_mon + 1)) {
		logger.log("WARN", "Given date has passed, request try again");
			return -5;
		}
		else if (month == (ltm->tm_mon + 1)) {
			if (day < ltm->tm_mday) {
				logger.log("WARN", "Given date has passed, request try again");
				return -5;
			}
			else if (day == ltm->tm_mday) {
				if (hour < ltm->tm_hour) {
					logger.log("WARN", "Given date has passed, request try again");
					return -5;
				}
				else if (hour == ltm->tm_hour) {
					if (minute < ltm->tm_min) {
						logger.log("WARN", "Given date has passed, request try again");
						return -5;
					}
				}
			}
		}
	}
	
	logger.log("INFO", "Date is valid");
	return 0;
}
string AlarmList::setAlarmSetting(const int option, const string alarm){ //returns a string representing the alarm schedule
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
		cout << "\tEnter a date (DD/MM/YYYY): ";
		getline(cin, date);
		while (AlarmList::checkDate(date, alarm)) {
			cerr << "\tPlease enter a valid date (DD/MM/YYYY): ";
			getline(cin, date);
		}
		logger.log("INFO", "Returned a date for alarm schedule");
		return date;	//will return a date with values separated by '/'
	}
	else {
        const int daysInWeek = 7;
        const string days[daysInWeek] = {"\tSunday: ", "\tMonday: ", "\tTuesday: ", "\tWednesday: ", "\tThursday: ", "\tFriday: ", "\tSaturday: "};

        while (daysOfWeek == "" || daysOfWeek == "0000000") {    //"0000000" is an invalid schedule
            if (daysOfWeek == "0000000") {
                cout << "\n\tYou must choose at least one day!" << endl;
            }
            //loop will ask for y/n for each day of the week and then add a '1' or '0' to the string accordingly
			daysOfWeek = "";
            cout << "\tSelect the days you wish the alarm to sound (Y/N)." << endl;
            for (int i = 0; i < daysInWeek; i++) {
                //get the input
                cout << days[i];
                getline(cin, input);
                while (AlarmList::checkYesOrNo(input)) {
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
    }
	
	//if 1, 2, 3, or 5
	logger.log("INFO", "Returned a days of week schedule for schedule");
	return daysOfWeek;
}


int AlarmList::delAlarm(int pos){ //deletes an alarm and writes the changes to the file
	if (pos < 0 ||pos >=length){
		logger.log("EROR","Attempted to delete an alarm at a non existent position."); 
		return -1;
	}
	Alarm* newAlarms = new Alarm[length - 1];
	for (int i = 0; i < pos; i++) {
		newAlarms[i].setAlarmName(alarms[i].getAlarmName());
		newAlarms[i].setAlarmTime(alarms[i].getAlarmTime());
		newAlarms[i].setAlarmSchedule(alarms[i].getAlarmSchedule());
	}
	for (int i = pos+1; i < length; i++) {
		newAlarms[i-1].setAlarmName(alarms[i].getAlarmName());
		newAlarms[i-1].setAlarmTime(alarms[i].getAlarmTime());
		newAlarms[i-1].setAlarmSchedule(alarms[i].getAlarmSchedule());
	}
	length--;
	
	alarms = newAlarms;
	writeList();
	logger.log("INFO", "Alarm deleted successfully");

	return 0;
}
int AlarmList::gpioSetup(const int pinNum, int &rq, const int pinMode) { //requests pins for use
	string pinN= to_string(pinNum);
	logger.log("TRCE","Attempting to set up pin "+pinN);
	int rv;
	// check if gpio is already exported
	if ((rq = gpio_is_requested(pinNum)) < 0) {
		cerr << "Error: GPIO pin " << pinNum << " is already in use."
			<< endl;
		return -1;
	}
	// export the gpio
	if (!rq) {
		if ((rv = gpio_request(pinNum, NULL)) < 0) {
			cerr << "Error: GPIO pin " << pinNum <<
				" could not be exported." << endl;
			return -1;
		}
	}
	// set to input direction
	if (pinMode == 0) {
		if ((rv = gpio_direction_input(pinNum)) < 0) {
			cerr << "Error: GPIO pin " << pinNum <<
				" could not be set as input." << endl;
			return -1;
		}
	} else {
		if ((rv = gpio_direction_output(pinNum, 0)) < 0) {
			cerr << "Error: GPIO pin " << pinNum <<
			       "could not be set as output." << endl;
			return -1;
		}
	}
	logger.log("INFO","Successfully set up pin "+pinN);
	return 0;
}
int AlarmList::gpioRelease(const int pinNum, int &rq) {//frees pins
	string pinN= to_string(pinNum);

	logger.log("TRCE","Attempting to release pin "+pinN);

	if (!rq) {
		if (gpio_free(pinNum) < 0) {
			cerr << "Error: Could not free GPIO pin " << pinNum <<
				endl;
			return -1;
		}
		return 0;
	}
	logger.log("INFO","Successfully released pin "+pinN);

	return 1;
}
