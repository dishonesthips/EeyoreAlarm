#include "Log.hpp"
#include "Mailer.hpp"
#include "Alarm.hpp"
#include <string>
#include <iostream>
#include <time.h>
#include <unistd.h>
#include <fstream>

using namespace std;

Log Alarm::logger;
Mailer Alarm::outMail("temp","temp");//declaring temporary mail class until we get user info in main func

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
			if (diff>=maxSecondsPlaying)
				outMail.sendTimeout(alarmName, diff);//email the user saying that their alarm timed out
			
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

		if (currMin == alarmTime && timeStruct->tm_sec<=1){//<=1 because we have observed it skipping seconds on rare occasions
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

void Alarm::setLogger(Log log){//assigns static logger member
	Alarm::logger = log;
}

void Alarm::setAlarmName(const string name){ //setter name
	alarmName = name;
}

void Alarm::setAlarmTime(const int aTime){ //setter time
	alarmTime = aTime;
}

void Alarm::setAlarmSchedule(const string sched){ //setter schedule
	schedule = sched;
	if (sched[2] == '/') { //if it is in date format, it must be a one time alarm
		oneTime = true;
	} else {
		oneTime = false;
	}
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

bool Alarm::getOneTime(){ //returns whether this is a one time alarm
	return oneTime;
}

void Alarm::writeStat(int day, int time){ //writes an integer to a statistics file
	ofstream statfile;
	string s = to_string(day);
	statfile.open("stats" + s + ".txt", ios::app | ios::out);
	statfile << time << "\r\n";
	statfile.close();
	logger.log("INFO","Successfully wrote a stat.");
}
