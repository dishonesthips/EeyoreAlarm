#ifndef ALARMSUITE
#define ALARMSUITE

#include <string>

using namespace std;

class Alarm{
	public:
		//static string DEFAULT_SONG_FILE;
		int tick();
		Alarm(int hour, int minute, string sch);
	private:
		//string songFile;
		int timeOngoing;
		bool ongoing;
		
		const int alarmTime;	
		const string schedule;
};

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
#endif