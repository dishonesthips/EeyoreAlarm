#ifndef ALARMLIST
#define ALARMLIST
#include <string>
#include "Alarm.hpp"

class AlarmList{
	public:
		static int checkRange(const std::string setting, const int lower, const int higher);
	
		const int EXIT_PIN = 0;
		const int TRIGGER_PIN = 1;
		const int BUZZER_PIN = 11;	
		
		const std::string filename = "alarms.txt";
		
		AlarmList(Log log);
		int runAlarm();
		int addAlarm();
		int delAlarm();
		int readList();
		int writeList();
		int displayList();
		
	private:
		Alarm* alarms;
		int length;
		
		int delAlarm(int pos);
		static bool isLeapYear(const int year);
		static int checkName(const std::string name);
		static int checkAlarm(const std::string alarm);
		static int checkDate(const std::string date, const std::string alarm);
		static int checkYesOrNo(const std::string yn);
		static std::string setAlarmSetting(const int option, const std::string alarm);
		
		int gpioSetup(const int pinNum, int &rq, const int pinMode);
		int gpioRelease(const int pinNum, int &rq);
		int rqExit;
		int rqTrigger;
		int rqBuzzer;
		
		Log logger;
};
#endif
