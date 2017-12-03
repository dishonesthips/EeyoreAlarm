#ifndef ALARM
#define ALARM
#include <string>
class Alarm{
	public:
		int tick(tm* timeStruct, int motionState); //return 1 or 0 if buzzer should be on/off for this alarm
		Alarm();
		void setAlarmName(const std::string name);
		void setAlarmTime(const int aTime);
		void setAlarmSchedule(const std::string sched);
				
		std::string getAlarmName();
		int getAlarmTime();
		std::string getAlarmSchedule();
		std::string printAlarm();
		
		std::string displayAlarm();
	private:
		tm* timeFreeze;//holds all information at the time of alarm start
		bool ongoing; //on if the buzzer should be on

		bool oneTime; //if it is a onetime alarm or a recurring alarm
		std::string alarmName; 
		int alarmTime; //minutes since midnight alarm will start
		std::string schedule; //represents either days to go off or a single date
};
#endif
