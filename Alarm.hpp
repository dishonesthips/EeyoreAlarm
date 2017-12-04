#ifndef ALARM
#define ALARM
#include <string>
class Alarm{
	public:
		static const int maxSecondsPlaying = 60;
		static Log logger;
		static Mailer outMail;
		
		Alarm();
		
		int tick(tm* timeStruct, int motionState); //return 1 or 0 if buzzer should be on/off for this alarm
		std::string printAlarm();//Gives alarm info in format to be used when writing to file
		std::string displayAlarm();//similar to printAlarm() but gives it in a more user friendly format
		void resetAlarm();//turns an alarm off

		//setters
		static void setLogger(Log log);
		void setAlarmName(const std::string name);
		void setAlarmTime(const int aTime);
		void setAlarmSchedule(const std::string sched);		
		
		//getters
		std::string getAlarmName();
		int getAlarmTime();
		std::string getAlarmSchedule();
		std::string getFormatTime();
		bool getOneTime();

		
	private:
		tm timeFreeze;//holds all information at the time of alarm start
		bool ongoing; //on if the buzzer should be on
		bool oneTime; //if it is a one time alarm or a recurring alarm
		std::string alarmName; 
		int alarmTime; //minutes since midnight alarm will start
		std::string schedule; //represents either days to go off or a single date (depends on oneTime)
		
		void writeStat(int day, int time);//appends a number to the stats file of the day the alarm went off
		
		
};
#endif
