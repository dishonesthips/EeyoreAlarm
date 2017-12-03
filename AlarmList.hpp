#ifndef ALARMLIST
#define ALARMLIST
#include <string>
#include "Alarm.hpp"

class AlarmList{
	public:
		const int EXIT_PIN = 0;
		const int TRIGGER_PIN = 1;
		const int BUZZER_PIN = 11;	
		
		const std::string filename = "alarms.txt";
		AlarmList();
		int runAlarm();
		int addAlarm();
		int delAlarm();
		int readList();
		int writeList();
		int displayList();
	private:
		Alarm* alarms;
		int length;
		
		static bool isLeapYear(const int year);
		static int checkName(const std::string name);
		static int checkAlarm(const std::string alarm);
		static int checkDate(const std::string date, const std::string alarm);
		static int checkYesOrNo(const std::string yn);
		static std::string setAlarmSetting(const int option, const std::string alarm);
};
#endif
