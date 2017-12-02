#include <iostream>
#include <string>
#include "AlarmSuite.h"
#include <time.h>

using namespace std;


class Alarm{
	public:
		int tick(tm* timeStruct);
		Alarm(int hour, int minute, string sched);
	private:
		tm* timeFreeze;
		bool ongoing;

		const bool oneTime;
		const int alarmTime;	
		const string schedule;
};
void Alarm::Alarm(int hour, int minute, string sched){
	alarmTime = hour*60 + minute;
	schedule = sched;
	if (sched.find('/')!=std::string::npos)//if a slash is found then it must be a single date
		oneTime = true;
	else
		oneTime = false;
	
	bool ongoing = false;
	int timeOngoing = 0;
}
int Alarm::tick(tm* timeStruct){
	currMin = (timeStruct->tm_hour)
	
	
}




class AlarmList{
	public:
		int Alarm_LL();
		int addAlarm(Alarm a);
		int delAlarm(int pos);
		int readList();
		int writeList();
		int displayList();
		
	private:
		Alarm* alarms;
		int length;
};
