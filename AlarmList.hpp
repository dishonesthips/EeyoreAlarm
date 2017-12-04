#ifndef ALARMLIST
#define ALARMLIST

#include "Alarm.hpp"
#include <string>

class AlarmList{
	public:
		static Log logger;
	
		const int EXIT_PIN = 0;
		const int TRIGGER_PIN = 1;
		const int BUZZER_PIN = 11;	
		
		const std::string filename = "alarms.txt";
		
		AlarmList();
		
		static int checkRange(const std::string setting, const int lower, const int higher);
		static int checkYesOrNo(const std::string yn);
		static void setLogger(Log log);

		void runAlarm();
		int addAlarm();
		int delAlarm();
		void displayList();
		
		int readList();
		int writeList();


	private:
		static bool isLeapYear(const int year);
		static int checkName(const std::string name);
		static int checkAlarm(const std::string alarm);
		static int checkDate(const std::string date, const std::string alarm);
		static std::string setAlarmSetting(const int option, const std::string alarm);
		
		int delAlarm(int pos);
		int gpioSetup(const int pinNum, int &rq, const int pinMode);
		int gpioRelease(const int pinNum, int &rq);

		Alarm* alarms;
		int length;
			
		int rqExit;
		int rqTrigger;
		int rqBuzzer;
		
};
#endif
