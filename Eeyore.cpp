#include <iostream>
#include <string>
#include <fstream>
#include <math.h>
#include <unistd.h>
#include <ugpio/ugpio.h>

#include "Log.hpp"
#include "UserInfo.hpp"
#include "Alarm.hpp"
#include "AlarmList.hpp"

using namespace std;

//Log member function declarations
Log::Log(){//constructor opens log file named with the time that the program was run
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
void Log::log(string severity, string message){ //logs a given message
	ofstream logfile; // declare the file object
	logfile.open(filename, ios::app | ios::out); // open the file
	
	//initialize time and set to local
	time_t now = time(0);
	tm* ltm = localtime(&now);
	
	//write data to log
	logfile << (ltm->tm_year + 1900) << "-" << (ltm->tm_mon + 1) << "-" << ltm->tm_mday << "\t" 
			<< ltm->tm_hour << ":" << ltm->tm_min << ":" << ltm->tm_sec << "\t(" << severity << "):\t " << message << "\r\n";
	logfile.close();
}


//UserInfo member function definitions
UserInfo::UserInfo(Log log){ //constructor

	name = "";
	email = "";
	logger = log;
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
		if (count == input.length()-1) {
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
string UserInfo::capitalize(string name){//capitalize name for format
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

//Alarm member function definitions
Alarm::Alarm(){//constructor
	alarmTime = -1;
	schedule = "";
	oneTime = false;
	ongoing = false;
	
}
int Alarm::tick(tm* timeStruct, int motionState){
	//determine if the alarm is due to go off sometime today
	bool goOffToday = false;
	
	if (oneTime){
		if	((stoi(schedule.substr(0,2)) == timeStruct->tm_mday) 
		    && (stoi(schedule.substr(3,2)) == (timeStruct->tm_mon+1))
			&& (stoi(schedule.substr(6,4)) == (1900+timeStruct->tm_year))){
				goOffToday = true;
		}
	}
	else{
		if(schedule[timeStruct->tm_wday] == '1')
			goOffToday = true;
	}
	if (!goOffToday)
		return 0;
	
	
	//if the alarm is going off sometime today
	if (ongoing){//if it is currently going
		int diff = difftime(mktime(timeStruct),mktime(&timeFreeze));
		cout<<"Alarm: \""<<alarmName<<"\" is currently active."<<endl;
		if (diff >= maxSecondsPlaying || motionState){
			cout<<"Alarm: \""<<alarmName<<"\" was deactivated in "<< diff<< " seconds.\n";
			writeStat(timeFreeze.tm_wday, diff);
			resetAlarm();
			
			return 0;
		}
		return 1;
	}
	else{//if it is not currently going
		int currMin = (timeStruct->tm_hour)*60 + timeStruct->tm_min;
		//cout<<currMin<<" "<<alarmTime<<endl;
		if (currMin == alarmTime && timeStruct->tm_sec==0){//time to go off is now
			ongoing = true;
			timeFreeze = *timeStruct;
			return 1;
		}
		return 0;
	}	
}
string Alarm::printAlarm(){//Gives alarm in format to be used when writing to file
	
	string s;
	string timeString = to_string(alarmTime);
	s = alarmName+","+timeString+","+schedule;//+"\r";
	return s;
}
void Alarm::setAlarmName(const string name){ //setter name
	alarmName = name;
}
void Alarm::setAlarmTime(const int aTime){ //setter time
	alarmTime = aTime;
}
void Alarm::setAlarmSchedule(const string sched){ //setter schedule
	schedule = sched;
	if (sched[2] == '/')
		oneTime = true;
}
string Alarm::getAlarmName(){ //getter name
	return alarmName;
}
int Alarm::getAlarmTime(){ //getter time
	return alarmTime;
}
string Alarm::getAlarmSchedule(){ //getter schedule
	return schedule;
}
string Alarm::getFormatTime(){
	string hour = to_string(alarmTime/60);
	if (hour.length() == 1) {
		hour = "0" + hour;
	}
	string minute = to_string(alarmTime%60);
	if (minute.length() == 1) {
		minute = "0" + minute;
	}
	return hour+":"+minute;
	
}
string Alarm::displayAlarm() {//similar to printAlarm() but gives it in a more user friendly format
	string formatSched = "\tName: " + alarmName + "\n";
	
	formatSched = formatSched + "\tTime: " + getFormatTime();
	
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
	}
	formatSched += "\n";

	return formatSched;
}
void Alarm::resetAlarm(){//resets alarm; called when an alarm needs to be turned off
	ongoing = false;
}
void Alarm::writeStat(int day, int time){
	ofstream statfile;
	string s = to_string(day);
	statfile.open("stats" + s + ".txt", ios::app | ios::out);
	statfile << time << "\r\n" << endl;
	statfile.close();
}
bool Alarm::getOneTime(){
	return oneTime;
}

//AlarmList member function declarations
AlarmList::AlarmList(Log log){
	alarms = NULL;
	int length = -1;
	logger = log;
	
	int exitRet = gpioSetup(EXIT_PIN, rqExit, 0);
	int trigRet = gpioSetup(TRIGGER_PIN, rqTrigger, 0);
	int buzzerRet = gpioSetup(BUZZER_PIN, rqBuzzer, 1);
	
	if ((exitRet + trigRet + buzzerRet) != 0)
		cerr <<"pin setup fucked up"<<endl;
	
	gpio_set_value(BUZZER_PIN, 0);

	
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
			int aTime = stoi(aTimeStr);
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
int AlarmList::runAlarm(){
	
	int exitVal;
	int triggerVal;

	bool exitButtonHit= false;
	const int sleepTime = 1;
	time_t now = time(0);
	tm* ltm = localtime(&now);

	int buzzerSum;
	
	/*
	cout << "\tYear: " << 1900 + ltm->tm_year<<endl;
	cout << "\tMonth: "<< 1 + ltm->tm_mon<< endl;
	cout << "\tDay: "<<  ltm->tm_mday << endl;
	cout << "\tDays since Sunday: " << ltm->tm_wday << endl;
	*/
	
	while(!exitButtonHit){
		buzzerSum = 0;
		now = time(0);
		tm* ltm = localtime(&now);
		
		sleep(sleepTime);
		cout << "\tCurrent time: " << (ltm->tm_hour) << ":" << (ltm->tm_min) << ":" << (ltm->tm_sec) << endl;
		
		exitVal = gpio_get_value(EXIT_PIN);
		triggerVal = gpio_get_value(TRIGGER_PIN);
		
		cout<<"exit value" << exitVal<<endl;
		cout<<"trigger value" << triggerVal<<endl;

		if(exitVal){
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
	cout<<"freeing pins"<<endl;
	gpioRelease(EXIT_PIN, rqExit);
	gpioRelease(TRIGGER_PIN, rqTrigger);
	gpioRelease(BUZZER_PIN, rqBuzzer);
	
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
	while (checkRange(setting,1,5)) {
		cout << "Please enter a single digit in range [1,5]: ";
		getline(cin, setting);
	}
	
	//implement setting
	const int option = setting[0] - '0';
	
	
	//cout << name<<" "<<(stoi(alarm.substr(0,2)) * 60 + stoi(alarm.substr(3,2)))<<" "<< AlarmList::setAlarmSetting(option, alarm) << endl;
	
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

	alarms = newAlarms;
	
	//cout << alarms[length].getAlarmName()<<" "<<alarms[length].getAlarmTime()<<" "<< alarms[length].getAlarmSchedule() << endl;
	
	
	writeList();
	//NICE
	
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
		//int count = 0;
		delAlarm(pos);
	}
	else{
		string s;
		cout<<"\n\tHit enter to continue... ";
		getline(cin, s);
	}
		
}
int AlarmList::delAlarm(int pos){
	if (pos < 0){
		//cerr
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
	return 0;
}
int AlarmList::displayList(){ // display list of alarms, user-friendly
	
	if (length){
		cout<<"\n\tThe following are your alarm(s):\n";
		for (int i = 0; i < length; i++) {
			cout << "\tALARM " << i+1 << "\n" << alarms[i].displayAlarm() << endl;
		}
	}
	else
		cout<<"\n\tIt seems like you have no alarms at the moment...\n";
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
		cout << "\tEnter a date (DD/MM/YYYY): ";
		getline(cin, date);
		while (AlarmList::checkDate(date, alarm)) {
			cerr << "\tPlease enter a valid date (DD/MM/YYYY): ";
			getline(cin, date);
		}
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
	const int day = stoi(date.substr(0,2));
	const int month = stoi(date.substr(3,2));
	const int year = stoi(date.substr(6,4));
	const int hour = stoi(alarm.substr(0,2));
	const int minute = stoi(alarm.substr(3,2));
	
	//cannot choose a date from the past
	if (year < (ltm->tm_year + 1900)) {
		cerr << "Choose a year in the future!" << endl;
		return -5;
	}
	if (month < (ltm->tm_mon + 1) && year == ltm->tm_year) {
		cerr << "Choose a month in the future!" << endl;
		return -5;
	}
	if (day < ltm->tm_mday && month == (ltm->tm_mon + 1)) {
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
int AlarmList::checkYesOrNo(const string yn){//error checks yes or no input
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
int AlarmList::checkRange(const string setting, const int lower, const int higher) {
	//check setting for empty string
	if (setting.empty()) {
		cerr << "Error: Empty string!" << endl;
		return -4;
	}
	
	for (int i = 0; i < setting.length(); i++) {
		if (setting[i]<'0' || setting[i]>'9') {
			cerr << "Dont use arrow keys, only input numbers" << endl;
			return -2;
		}
	}
	
	if (stoi(setting)< lower || stoi(setting) > higher) {
		cerr << "Error: Invalid option" << endl;
		return -4;
	}
	
	return 0;
}
int AlarmList::gpioSetup(const int pinNum, int &rq, const int pinMode) {
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
	return 0;
}
int AlarmList::gpioRelease(const int pinNum, int &rq) {
	if (!rq) {
		if (gpio_free(pinNum) < 0) {
			cerr << "Error: Could not free GPIO pin " << pinNum <<
				endl;
			return -1;
		}
		return 0;
	}
	return 1;
}


int main(const int argc, const char* const args[]){
	Log logger;

	UserInfo user= UserInfo(logger);
	AlarmList alarmList = AlarmList(logger);
	alarmList.readList();

	
	bool exit = false;
	
	if (user.fileNotExist()) {
		cout<<"\n\tWelcome to Eeyore! Is this your first time?\n\tI don't recognize you...\n\n";
		user.writeInfo();
		cout<<"\n\n";
	}
	user.readInfo();
	//cout << user.getName() << " " << user.getEmail() << endl;
	cout<<"\n\t_____________________________\n\n\n\n";

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
		
		while (AlarmList::checkRange(menuAnswer,1,7)) {
			cout << "\tPlease enter a single digit in range [1,7]: ";
			getline(cin, menuAnswer);
		}

		if(menuAnswer[0] == '1'){//Run Alarm
			alarmList.runAlarm();
		}

		else if(menuAnswer[0] == '2'){//Add an Alarm
			cout<<"\n\tAdding an Alarm-\n";
			alarmList.addAlarm();
		}
		else if(menuAnswer[0] == '3'){//Delete an Alarm
			cout<<"\n\tDeleting an Alarm-\n";
			alarmList.displayList();
			alarmList.delAlarm();
		}
		else if (menuAnswer[0] == '4') {//View Alarms
			alarmList.displayList();
			cout<<"\n\tHit enter to continue... ";
			getline(cin,menuAnswer);
		}
		else if(menuAnswer[0] == '5'){//Update User Info
			cout<<"\n\tUpdating User Info-\n";
			user.writeInfo();
			user.readInfo();
		}
		else if(menuAnswer[0] == '6'){//View Statistics
			
		}
		else if(menuAnswer[0] == '7'){//Exit
			logger.log("TRACE","Manual request to exit program");
			exit = true;
		}
		else{
			logger.log("WARNING","Error checking menuAnswer went wrong, treated as if exit request;");
			exit = true;
		}

		cout<<"\n\n\n\t_____________________________\n\n\n\n";
	}
	cout<<"\tThanks for using Eeyore! Sweet Dreams!\n"<<endl;
	return 0;

}
