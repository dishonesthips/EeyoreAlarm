#include <iostream>
#include <string>
#include <fstream>
#include <math.h>
using namespace std;

const string statFileName = "stats.txt";

//global function declarations
int log(const string message, const string severity);
int stringToInt(string str);
int checkRange(const string setting, const char lower, const char higher);
//class declarations
class UserInfo {
	public:
		const string filename = "userInfo.txt";
		UserInfo();	//declare constructor
		
		//declare functions
		bool fileNotExist();
		string getName();
		string getEmail();
		void writeInfo();
		void readInfo();
		
	private:
		//declare private variables
		string name;
		string email;
		
		//error check methods do not require an instance of a class
		string capitalize(string name);
		static int checkName(const string input);
		static int checkEmail(const string input);
};
class Alarm{
	public:
		int tick(tm* timeStruct); //return 1 or 0 if buzzer should be on/off for this alarm
		Alarm();
		string printAlarm();
		void setAlarmName(const string name);
		void setAlarmTime(const int aTime);
		void setAlarmSchedule(const string sched);
		
		//void displayAlarm();
		
		string getAlarmName();
		int getAlarmTime();
		string getAlarmSchedule();
		string displayAlarm();
	private:
		tm* timeFreeze;//holds all information at the time of alarm start
		bool ongoing;

		bool oneTime;
		string alarmName;
		int alarmTime; //minutes since midnight alarm will start
		string schedule; //represents either days to go off or a single date
};
class AlarmList{
	public:
		const string filename = "alarms.txt";
		AlarmList();
		int addAlarm();
		int delAlarm();
		int readList();
		int writeList();
		int displayList();
		
		Alarm* alarms;
		int length;
		
	private:
		static bool isLeapYear(const int year);
		static int checkName(const string name);
		static int checkAlarm(const string alarm);
		static int checkDate(const string date, const string alarm);
		static int checkYesOrNo(const string yn);
		static string setAlarmSetting(const int option, const string alarm);
};


//UserInfo member function declarations
UserInfo::UserInfo(){ //constructor

	name = "";
	email = "";
}
void UserInfo::writeInfo(){//write info to a file with paramters info
	//declare strings to be written to file
	string nameFile;
	string emailFile;
	
	//get name
	cout << "\tWhat is your name? ";
	getline(cin, nameFile);
	while (UserInfo::checkName(nameFile)) {
		cout << "\tPlease enter a valid name: ";
		getline(cin, nameFile);
	}
	nameFile = capitalize(nameFile);

	//get email
	cout << "\tWhat is your email? ";
	getline(cin, emailFile);
	while (UserInfo::checkEmail(emailFile)) {
		cout << "\tPlease enter a valid email: ";
		getline(cin, emailFile);
	}
	//write to file
	ofstream outfile;
	outfile.open(filename);
	outfile << nameFile << "," << emailFile << "\r\n";
	outfile.close();
}
void UserInfo::readInfo(){//read info from a file and sets values of object UserInfo accordingly

	//declare variable
	string line;
	
	//reset string name and email to empty string since they will be appended to
	name = "";
	email = "";
	
	//read line from text file
	ifstream infile;
	infile.open(filename);
	getline(infile, line);
	infile.close();
	
	//loop through individual characters of the line, appending to name and email accordingly
	int i = 0;
	while (line[i] != ',') {
		name += line[i];
		i++;
	}
	i++;
	while (line[i]) {
		email += line[i];
		i++;
	}
}
bool UserInfo::fileNotExist(){//check to see if text file already exists
	ifstream infile;
	infile.open(filename);
	if(infile.is_open()) {
		infile.close();
		return false;	//file exists
	}
	infile.close();
	return true; //file does not exist (yet) :smirk:
}
int UserInfo::checkName(const string input){//static error check valid name
	//check for empty string
	if (input.empty()) {
		cerr << "Error: Empty string!" << endl;
		return -2;
	}
	/*
	//test print name COMMENT OUT OF FINAL COPY
	cout << "Recieved name: ";
	for (int i = 0; i < input.length(); i++) {
		cout << input[i] << " ";
	}
	cout << endl;
	*/
	//check for  (weird bug involving cin operator)
	for (int i = 0; i < input.length(); i++) {
		if (input[i] == '') {
			cerr << "Don't use arrow keys!" << endl;
			return -2;
		}
	}
	
	//valid name
	return 0;
}
int UserInfo::checkEmail(const string input){//static error check valid email
	//check for empty string
	if (input.empty()) {
		cerr << "Error: Empty string!" << endl;
		return -3;
	}
	
	int count = 0;
	/*
	//test print email COMMENT OUT OF FINAL COPY
	cout << "Recieved email: ";
	for (int i = 0; i < input.length() + 1; i++) {
		cout << input[i] << " ";
	}
	cout << endl;
	*/
	//error checking for email name validity
	if (input[count] == '.') {
		cerr << "Error: Invalid dot usage" << endl;
		return -3;	//email cannot begin with a '.'
	}
	
	//check for  (weird bug involving cin operator)
	for (int i = 0; i < input.length(); i++) {
		if (input[i] == '') {
			cerr << "Don't use arrow keys!" << endl;
			return -2;
		}
	}
	
	//first find an '@'; if cannot find within length of email characters, error
	//the local-part of an email can only include certain characters; if invalid character, error
	while (input[count] != '@') {
		if (count > input.length()) {
			cerr << "Error: Invalid email. Cannot find '@'" << endl;
			return -3;
		}
		if ((input[count] != '!' && (input[count] < '#' || input[count] > 39) && 
		input[count] != '*' && input[count] != '+' && (input[count] < '-' || input[count] > '/') && 
		input[count] != '=' && input[count] != '?' && (input[count] < '^' || input[count] > '~') &&
		(input[count] > '{' || input[count] < '~')) && (input[count] < 'A' || input[count] > 'Z') &&
		(input[count] < '0' || input[count] > '9') && input[count]) {
			cerr << "Error: Invalid character" << endl;
			return -3;
		}
		if (input[count] == '.' && (input[count - 1] == '.' || input[count + 1] == '.')) {			//cannot have two dots in a row
			cerr << "Error: Invalid dot usage" << endl;
			return -3;
		}
		count++;
	}
	count++;	//can only have one '@', increment count once after the single @ is found
	
	//the domain part of an email has a separate set of valid characters
	//at least one '.' is required for a valid domain
	while (input[count] != '.') {
		if (((input[count] < 'A' || input[count] > 'Z') && (input[count] < 'a' || input[count] > 'z') &&
		(input[count] < '0' || input[count] > '9') && input[count] != '-')) {
			cerr << "Error: Invalid character" << endl;
			return -3;
		}
		count++;
	}
	
	//the rest of the domain can be any valid characters, however cannot end in a '.'
	while (input[count]) {
		if (((input[count] < 'A' || input[count] > 'Z') && (input[count] < 'a' || input[count] > 'z') &&
		(input[count] < '0' || input[count] > '9') && input[count] != '-') && input[count] != '.') {
			cerr << "Error: Invalid character" << endl;
			return -3;
		}
		if (input[count] == '.' && (input[count - 1] == '.' || input[count + 1] == '.' || input[count + 1] == 0)) {
			cerr << "Error: Invalid dot usage" << endl;
			return -3;
		}
		count++;
	}
	
	//valid email
	return 0;
}
string UserInfo::capitalize(string name){//BUG THAT IT DECAPITALIZES IF ALREADY CAPITALIZEDcapitalize name for format
	//declare new name to be capitalized
	string newName = name;
	
	//capitalize only lowercase letters that come after a space
	//set other letters to lowercase
	for (int i = 0; i < newName.length(); i++) {
		if ((i == 0 || newName[i - 1] == ' ' || newName[i - 1] == '-' || newName[i - 1] == '\'') && newName[i] >= 'a' && newName[i] <= 'z')
			newName[i] = newName[i] - 32;
		else if (newName[i] >= 'A' && newName[i] <= 'Z' && (i != 0 && newName[i - 1] != ' ' && newName[i - 1] != '-' && newName[i - 1] != '\''))
			newName[i] = newName[i] + 32;
	}
	//return capitalized name
	return newName;
}
string UserInfo::getName(){ //getter name
	return name;
}
string UserInfo::getEmail(){//getter email
	return email;
}

Alarm::Alarm(){
	alarmTime = -1;
	schedule = "";
	oneTime = false;
	ongoing = false;
	timeFreeze=NULL;
	
}
int Alarm::tick(tm* timeStruct){

	int currMin = (timeStruct->tm_hour);
	
	
}
string Alarm::printAlarm(){
	
	string s;
	string timeString = to_string(alarmTime);
	s = alarmName+","+timeString+","+schedule;//+"\r";
	return s;
}
void Alarm::setAlarmName(const string name){
	alarmName = name;
}
void Alarm::setAlarmTime(const int aTime){
	alarmTime = aTime;
}
void Alarm::setAlarmSchedule(const string sched){
	schedule = sched;
	if (sched[2] == '/')
		oneTime = true;
}
string Alarm::getAlarmName(){
	return alarmName;
}
int Alarm::getAlarmTime(){
	return alarmTime;
}
string Alarm::getAlarmSchedule(){
	return schedule;
}
string Alarm::displayAlarm() {
	string formatSched = "\tName: " + alarmName + "\n";
	
	string hour = to_string(alarmTime/60);
	if (hour.length() == 1) {
		hour = "0" + hour;
	}
	string minute = to_string(alarmTime%60);
	if (minute.length() == 1) {
		minute = "0" + minute;
	}
	formatSched = formatSched + "\tTime: " + hour + ":" + minute;
	
	const int daysInWeek = 7;
	string daysWeekStr[daysInWeek] = {"SU", "MO", "TU", "WE", "TH", "FR", "SA"};
	
	if (oneTime)
		formatSched = formatSched + "\tDate: " + schedule;
	else {
		formatSched = formatSched + "\tSchedule: ";
		for (int i = 0; i < daysInWeek; i++) {
			if (schedule[i] == '1') {
				formatSched = formatSched + daysWeekStr[i] + " ";
			}
		}
		formatSched += "\n";
	}
	
	return formatSched;
}

AlarmList::AlarmList(){
	alarms = NULL;
	int length = -1;
}
int AlarmList::readList(){ //creates list of alarms from text file. only to be run once at start of program
	
	string line;
	length = 0;
	
	//opening file to get number of lines
	ifstream infile;            // declare the file object
	infile.open(filename);      // open the file
	
	if (!infile.is_open()){
		cerr<<"cant find alarm file oh no"<<endl;
		return -1;
	}
	
	while(!infile.eof()){
		getline(infile,line);
		if(!line.empty() && line.compare("\r")!=0)
			length++;
	}
	infile.close();
	
	

	Alarm* tmp = alarms;
	alarms = new Alarm[length];
	delete tmp;
	

	ifstream infileR;            // declare the file object
	infileR.open(filename);      // open the file

	if (!infileR.is_open()){
		cerr<<"cant find alarm file oh no"<<endl;
		return -1;
	}
	
	
	int index = 0;
	int charIndex;
	while(!infileR.eof()){
		getline(infileR,line);
		if(!line.empty() && line.compare("\r")!=0){
			string aName = "";
			string aTimeStr="";
			string aSched="";
			//cout<<index<<endl;
			//AlarmList::assignData(line,alarms[index++]);
			charIndex = 0;
			while (line[charIndex] != ',') {
				aName+= line[charIndex];
				charIndex++;
			}
			charIndex++;
			
			while (line[charIndex] != ',') {
				aTimeStr += line[charIndex];
				charIndex++;
			}
			int aTime = stringToInt(aTimeStr);
			charIndex++;
			
			while (line[charIndex]) {
				aSched += line[charIndex];
				charIndex++;
			}
			charIndex++;
			//cout<<aName<<" "<<aTime<<" "<<aSched<<endl;
			alarms[index].setAlarmName(aName);
			alarms[index].setAlarmTime(aTime);
			alarms[index].setAlarmSchedule(aSched);
			
			index++;
		}
		

	}
	infileR.close();
	return 0;

}
int AlarmList::writeList(){ //appends an alarm to the file of alarms
	
	
	if (length > 0) {
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
	
	return 0;
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
	while (checkRange(setting,'1','5')) {
		cout << "Please enter a single digit in range [1,5]: ";
		getline(cin, setting);
	}
	
	//implement setting
	const int option = setting[0] - '0';
	
	
	//cout << name<<" "<<(stringToInt(alarm.substr(0,2)) * 60 + stringToInt(alarm.substr(3,2)))<<" "<< AlarmList::setAlarmSetting(option, alarm) << endl;
	
	Alarm* newAlarms = new Alarm[length + 1];	
	
	for (int i = 0; i < length; i++){
		newAlarms[i].setAlarmName(alarms[i].getAlarmName());
		newAlarms[i].setAlarmTime(alarms[i].getAlarmTime());
		newAlarms[i].setAlarmSchedule(alarms[i].getAlarmSchedule());
	}
	newAlarms[length].setAlarmName(name);
	newAlarms[length].setAlarmTime(stringToInt(alarm.substr(0,2)) * 60 + stringToInt(alarm.substr(3,2)));
	newAlarms[length].setAlarmSchedule(AlarmList::setAlarmSetting(option, alarm));
	length++;

	alarms = newAlarms;
	
	//cout << alarms[length].getAlarmName()<<" "<<alarms[length].getAlarmTime()<<" "<< alarms[length].getAlarmSchedule() << endl;
	
	
	writeList();
	//NICE
	
	return 0;
}
int AlarmList::delAlarm(){ //remove an alarm from the list of alarms
	string posStr;
	char maxLengthChar = length + '0';
	cout << "\tSelect which alarm to delete (number): ";
	getline(cin, posStr);
	while (checkRange(posStr, '1', maxLengthChar)) {
		cerr << "\tPlease enter a single digit in range [1," << length << "]: " << endl;
		getline(cin, posStr);
	}
	int pos = stringToInt(posStr)-1;
	int count = 0;
	cout << "HI" << endl;
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
	
}
int AlarmList::displayList() { // display list of alarms, user-friendly
	cout<<"\n\tThe following are your alarms:\n";
	for (int i = 0; i < length; i++) {
		cout << "\tALARM " << i+1 << "\n" << alarms[i].displayAlarm() << endl;
	}
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
string AlarmList::setAlarmSetting(const int option, const string alarm){
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
		while (AlarmList::checkDate(date, alarm)) {
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
	
	//if 1, 2, 3, or 5
	return daysOfWeek;
}
int AlarmList::checkName(const string input){//error checks for empty string and issues with cin
	//check for empty string
	if (input.empty()) {
		cerr << "Error: Empty string!" << endl;
		return -2;
	}
	/*
	//test print name COMMENT OUT OF FINAL COPY
	cout << "Recieved name: ";
	for (int i = 0; i < input.length(); i++) {
		cout << input[i] << " ";
	}
	cout << endl;
	*/
	//check for  (weird bug involving cin operator)
	for (int i = 0; i < input.length(); i++) {
		if (input[i] == '') {
			cerr << "Don't use arrow keys!" << endl;
			return -2;
		}
	}
	
	//valid name
	return 0;
}
int AlarmList::checkAlarm(const string alarm){
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
int AlarmList::checkDate(const string date, const string alarm){
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
	
	//initialize time and set to local to compare dates
	time_t now = time(0);
	tm* ltm = localtime(&now);
	
	//declare day, month, year, hour, minute ints
	const int day = stringToInt(date.substr(0,2));
	const int month = stringToInt(date.substr(3,2));
	const int year = stringToInt(date.substr(6,4));
	const int hour = stringToInt(alarm.substr(0,2));
	const int minute = stringToInt(alarm.substr(3,2));
	
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
	
	//check for valid time and year
	const int numMonths = 12;
	const int daysInMonths[numMonths] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	
	//check for zeroes
	if (day == 0 || month == 0 || year == 0) {
		
		cerr << "Error: Invalid date" << endl;
		return -5;
	}
	
	//check for bad month
	if (month > 12) {
		cerr << "Error: Invalid date" << endl;
		return -5;
	}
	
	//check leap year as an outlier
	else if (AlarmList::isLeapYear(year) && month == 2 && day > 29) {
		cerr << "Error: Invalid date" << endl;
		return -5;
	}
	else if (!AlarmList::isLeapYear(year) && month == 2 && day > 28) {
		cerr << "Error: Invalid date" << endl;
		return -5;
	}
	
	//check each day for validitiy with respect to month
	for (int i = 0; i < numMonths; i++) {
		if (month == (i + 1) && day > daysInMonths[i]) {
			cerr << "Invalid date" << endl;
			return -5;
		}
	}
	
	//freak off???
	return 0;
}
int AlarmList::checkYesOrNo(const string yn) {//error checks yes or no input
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

//global function definitions
int checkRange(const string setting, const char lower, const char higher) {
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
	
	//1 2 3 4 5
	return 0;
}
int log(const string severity, const string message) {
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
			<< ltm->tm_hour << ":" << ltm->tm_min << ":" << ltm->tm_sec << "\t(" << severity << "):\t " << message << "\r\n";
	logfile.close();
	
	return 0;
}
int stringToInt(const string str){
	int num = 0;
	int count = 0;
	
	for (int i = str.length() - 1; i >= 0; i--) {
		num = num + (str[i] - '0') * pow(10, count);
		count++;
	}
	
	return num;
}
int main(const int argc, const char* const args[]){
	bool exit = false;
	UserInfo user;
	AlarmList alarmList;
	alarmList.readList();
	
	if (user.fileNotExist()) { 
		cout<<"\n\tWelcome to Eeyore! Is this your first time?\n\tI don't recognize you...\n\n";
		user.writeInfo();
	}
	user.readInfo();
	//cout << user.getName() << " " << user.getEmail() << endl;

	while (!exit){
		
		
		cout<<"\tWelcome to Eeyore, "<<user.getName()<<"\n\n\t"
			<<"1. Run Alarm System\n\t"
			<<"2. Add an Alarm\n\t"
			<<"3. Delete an Alarm\n\t"
			<<"4. View Alarms\n\t"
			<<"5. Update User Info\n\t"
			<<"6. View Statistics\n\t"
			<<"7. Exit\n\n\t"
			<<"Please enter the number corresponding to one of the options: ";

		string menuAnswer;
		getline(cin, menuAnswer);
		
		while (checkRange(menuAnswer,'1','7')) {
			cout << "\tPlease enter a single digit in range [1,7]: ";
			getline(cin, menuAnswer);
		}
		
		
	

		if(menuAnswer[0] == '1'){//Run Alarm
		}

		else if(menuAnswer[0] == '2'){//Add an Alarm
			alarmList.addAlarm();
		}
		else if(menuAnswer[0] == '3'){//Delete an Alarm
			alarmList.displayList();
			alarmList.delAlarm();
		}
		else if (menuAnswer[0] == '4') {//View Alarms
			alarmList.displayList();
			cout<<"\tHit enter to continue... ";
			getline(cin,menuAnswer);
		}
		else if(menuAnswer[0] == '5'){//Update User Info
			user.writeInfo();
			user.readInfo();
		}
		else if(menuAnswer[0] == '6'){//View Statistics
			
		}
		else if(menuAnswer[0] == '7'){//Exit
			log("TRACE","Manual request to exit program");
			exit = true;
		}
		else{
			log("WARNING","Error checking menuAnswer went wrong, treated as if exit request;");
			exit = true;
		}

		cout<<"\n\n\n\t_____________________________\n\n\n";
	}
	cout<<"\n\tThanks for using Eeyore! Sweet Dreams!"<<endl;
	return 0;

}
