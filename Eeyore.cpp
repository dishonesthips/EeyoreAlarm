#include <iostream>
#include <string>
#include <fstream>
#include <math.h>
#include <unistd.h>
#include <ugpio/ugpio.h>

using namespace std;


//class declarations
class Log {
	public:
		Log();
		void log(string, string);
		
	private:
		string filename;
};
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
		static void setLogger(Log log);
		
		
		static Log logger;

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
		static const int maxSecondsPlaying = 60;
		
		Alarm();
		void resetAlarm();		
		static void setLogger(Log log);
		static Log logger;


		int tick(tm* timeStruct, int motionState); //return 1 or 0 if buzzer should be on/off for this alarm
		
		void setAlarmName(const string name);
		void setAlarmTime(const int aTime);
		void setAlarmSchedule(const string sched);		
		string getAlarmName();
		int getAlarmTime();
		string getAlarmSchedule();
		bool getOneTime();
		string getFormatTime();

		
		string printAlarm();
		string displayAlarm();
	private:
		tm timeFreeze;//holds all information at the time of alarm start
		bool ongoing; //on if the buzzer should be on
		bool oneTime; //if it is a onetime alarm or a recurring alarm
		string alarmName; 
		int alarmTime; //minutes since midnight alarm will start
		string schedule; //represents either days to go off or a single date
		
		void writeStat(int day, int time);
		
		
};
class AlarmList{
	public:
		static int checkRange(const string setting, const int lower, const int higher);

	
		const int EXIT_PIN = 0;
		const int TRIGGER_PIN = 1;
		const int BUZZER_PIN = 11;	

		
		const string filename = "alarms.txt";
		
		AlarmList();
		int runAlarm();
		int addAlarm();
		int delAlarm();
		int readList();
		int writeList();
		int displayList();
		static Log logger;

		static void setLogger(Log log);

	private:
		Alarm* alarms;
		int length;
		
		int delAlarm(int pos);
		static bool isLeapYear(const int year);
		static int checkName(const string name);
		static int checkAlarm(const string alarm);
		static int checkDate(const string date, const string alarm);
		static int checkYesOrNo(const string yn);
		static string setAlarmSetting(const int option, const string alarm);
		
		int gpioSetup(const int pinNum, int &rq, const int pinMode);
		int gpioRelease(const int pinNum, int &rq);
		int rqExit;
		int rqTrigger;
		int rqBuzzer;
		
};
class ReadStat {
	public:
		ReadStat(const string);
		void readData();
		void specialDataLen(const int);
		void specialData(const int, const int*);
		void setMax();
		void setMin();
		void setMean();
		void sort();
		void setMedian();
		void histogram();
		bool fileExists();
		string* getStats();
		void displayStats();
		void writeStats(const string*, const string);
		int getLength();
		int* getData();
		const int bucketsLen = 8;
		
		static Log logger;

	private:
		string filename;
		int* data;
		int length;
		int max;
		int min;
		float mean;
		int median;
		int buckets[8];
};
class ReadStatList {
	public:
		static Log logger;

		ReadStatList();
		int runStats();
		static void setLogger(Log log);

		const int length = 10;
		const int lengthDays = 7;
	private:
		ReadStat* stats[10];
};


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


//UserInfo member definitions
Log UserInfo::logger;
UserInfo::UserInfo(){//constructor
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
	
	logger.log("INFO", "User info writing successful");
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
	
	logger.log("INFO", "User name and email read successfully");
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
		logger.log("WARN", "Given empty string for name, request try again");
		return -2;
	}
	
	//check for  (weird bug involving cin operator)
	for (int i = 0; i < input.length(); i++) {
		if (input[i] == '') {
			logger.log("WARN", "Given arrow key input for name, request try again");
			return -2;
		}
	}
	
	//valid name
	logger.log("INFO", "Name is valid");
	return 0;
}
int UserInfo::checkEmail(const string input){//static error check valid email
	//check for empty string
	if (input.empty()) {
		logger.log("WARN", "Given empty string for email, request try again");
		return -3;
	}
	
	int count = 0;

	//error checking for email name validity
	if (input[count] == '.') {
		logger.log("WARN", "Given an email starting with '.', request try again");
		return -3;	//email cannot begin with a '.'
	}
	
	//check for  (weird bug involving cin operator)
	for (int i = 0; i < input.length(); i++) {
		if (input[i] == '') {
			logger.log("WARN", "Given arrow key input for email, request try again");
			return -2;
		}
	}
	
	//first find an '@'; if cannot find within length of email characters, error
	//the local-part of an email can only include certain characters; if invalid character, error
	while (input[count] != '@') {
		if (count == input.length()-1) {
			logger.log("WARN", "Given an email without an '@', request try again");
			return -3;
		}
		if ((input[count] != '!' && (input[count] < '#' || input[count] > 39) && 
		input[count] != '*' && input[count] != '+' && (input[count] < '-' || input[count] > '/') && 
		input[count] != '=' && input[count] != '?' && (input[count] < '^' || input[count] > '~') &&
		(input[count] > '{' || input[count] < '~')) && (input[count] < 'A' || input[count] > 'Z') &&
		(input[count] < '0' || input[count] > '9') && input[count]) {
			logger.log("WARN", "Given an email with an invalid character in it");
			return -3;
		}
		if (input[count] == '.' && (input[count - 1] == '.' || input[count + 1] == '.')) {			//cannot have two dots in a row
			logger.log("WARN", "Given an email with two dots in a row, request try again");
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
			logger.log("WARN", "Given an email with an invalid character in it, request try again");
			return -3;
		}
		count++;
	}
	
	//the rest of the domain can be any valid characters, however cannot end in a '.'
	while (input[count]) {
		if (((input[count] < 'A' || input[count] > 'Z') && (input[count] < 'a' || input[count] > 'z') &&
		(input[count] < '0' || input[count] > '9') && input[count] != '-') && input[count] != '.') {
			logger.log("WARN", "Given an email with an invalid character in it, request try again");
			return -3;
		}
		if (input[count] == '.' && (input[count - 1] == '.' || input[count + 1] == '.' || input[count + 1] == 0)) {
			logger.log("WARN", "Given an email ending with a '.', request try again");
			return -3;
		}
		count++;
	}
	
	//valid email
	logger.log("INFO", "Email is valid");
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
	
	logger.log("INFO", "Name was formatted successfully");
}
string UserInfo::getName(){ //getter name
	return name;
}
string UserInfo::getEmail(){//getter email
	return email;
}
void UserInfo::setLogger(Log log){//assigns static logger member
	UserInfo::logger = log;
}

//Alarm member  definitions
Log Alarm::logger;
Alarm::Alarm(){//constructor
	alarmTime = -1;
	schedule = "";
	oneTime = false;
	ongoing = false;
	
}
int Alarm::tick(tm* timeStruct, int motionState){ //returns 1 if buzzer should be currently going off, else 0
	//determine if the alarm is due to go off sometime today
	bool goOffToday = false;
	if (oneTime){//if it's a one time alarm we must check the schedule considering it is in DD/MM/YYYY format
		if	((stoi(schedule.substr(0,2)) == timeStruct->tm_mday) 
		    && (stoi(schedule.substr(3,2)) == (timeStruct->tm_mon+1))
			&& (stoi(schedule.substr(6,4)) == (1900+timeStruct->tm_year))){
				goOffToday = true;
		}
	}
	else{//if it is a recurring alarm it is in a binary (eg 0010010) format
		if(schedule[timeStruct->tm_wday] == '1')
			goOffToday = true;
	}
	if (!goOffToday)
		return 0;//buzzer will definitely not go off if the alarm isn't going off today
	
	
	//if the alarm is going off sometime today
	
	if (ongoing){//if it is currently going
		int diff = difftime(mktime(timeStruct),mktime(&timeFreeze)); //returns difference in seconds between 2 time structs
		cout<<"\tAlarm: \""<<alarmName<<"\" is currently active."<<endl;
		if (diff >= maxSecondsPlaying || motionState){ //either the time is up or it has been deactivated
			cout<<"\tAlarm: \""<<alarmName<<"\" was deactivated in "<< diff<< " seconds.\n";
			writeStat(timeFreeze.tm_wday, diff); //write how long it was on to a stats file
			resetAlarm();
			logger.log("INFO","Alarm: \""+alarmName+"\" successfully deactivated.");

			return 0;
		}
		return 1;
	}
	else{//if it is not currently going
		int currMin = (timeStruct->tm_hour)*60 + timeStruct->tm_min;
		if (currMin == alarmTime && timeStruct->tm_sec==0){//time to go off is now
			ongoing = true;
			timeFreeze = *timeStruct;
			logger.log("INFO","Alarm: \""+alarmName+"\" successfully activated.");
			return 1;
		}
		return 0;
	}	
}
string Alarm::printAlarm(){//Gives alarm in format to be used when writing to file
	string s;
	string timeString = to_string(alarmTime);
	s = alarmName+","+timeString+","+schedule;
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
	if (sched[2] == '/') //if it is in date format, it must be a one time alarm
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
string Alarm::getFormatTime(){ //getter time in a string format (eg. 01:05)
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
void Alarm::writeStat(int day, int time){ //writes an integer to a statistics file
	ofstream statfile;
	string s = to_string(day);
	statfile.open("stats" + s + ".txt", ios::app | ios::out);
	statfile << time << "\r\n";
	statfile.close();
	logger.log("INFO","Successfully wrote a stat.");
}
bool Alarm::getOneTime(){ //returns whether this is a one time alarm
	return oneTime;
}
void Alarm::setLogger(Log log){//assigns static logger member
	Alarm::logger = log;
}

//AlarmList member function declarations
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
int AlarmList::runAlarm(){//runs in a loop handling the alarm system and alarms themselves
	
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
		if (ltm->tm_hour % 10 == 0)
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
void AlarmList::setLogger(Log log){ //assigns static logger member
	AlarmList::logger = log;
}
//Readstat member function declarations
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
			buckets[((data[i] - 1) / 60) + 2]++;
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
	logger.log("INFO", "Writing and printing stats success");
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

//ReadStatList member functions
Log ReadStatList::logger;
ReadStatList::ReadStatList() {
	string num = "";
	//populate array of stats, 0-6 are normal, 7-9 are special
	for (int i = 0; i < length; i++) {
		num = to_string(i);
		stats[i] = new ReadStat("stats" + num + ".txt");
	}

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
	return 0;
}
void ReadStatList::setLogger(Log log){//logger
	ReadStatList::logger = log;
}

int main(const int argc, const char* const args[]){
	Log logger;
	UserInfo::setLogger(logger);
	Alarm::setLogger(logger);
	AlarmList::setLogger(logger);
	ReadStatList::setLogger(logger);

	
	
	UserInfo user= UserInfo();
	AlarmList alarmList = AlarmList();

	alarmList.readList();

	ReadStatList stats = ReadStatList();

	
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
			stats.runStats();
			cout<<"\n\tHit enter to continue... ";
			getline(cin,menuAnswer);
		}
		else if(menuAnswer[0] == '7'){//Exit
			logger.log("TRCE","Manual request to exit program");
			exit = true;
		}
		else{
			logger.log("WARN","Error checking menuAnswer went wrong, treated as if exit request;");
			exit = true;
		}

		cout<<"\n\n\n\t_____________________________\n\n\n\n";
	}
	cout<<"\tThanks for using Eeyore! Sweet Dreams!\n"<<endl;
	return 0;

}
