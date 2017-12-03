#include "Alarm.hpp"
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

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
