#include "AlarmList.hpp"
#include <string>

using namespace std;

//Alarm member function definitions
Alarm::Alarm(){
	alarmTime = -1;
	schedule = "";
	oneTime = false;
	ongoing = false;
	timeFreeze=NULL;
	
}
int Alarm::tick(tm* timeStruct, int motionState){

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
