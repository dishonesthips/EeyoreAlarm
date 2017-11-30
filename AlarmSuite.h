#ifndef ALARMSUITE
#define ALARMSUITE

#include <string>

using namespace std;

class Alarm{
	public:
		//static string DEFAULT_SONG_FILE;
		int tick();
		Alarm(int hour, int minute, bool m, bool t, bool w, bool th, bool f, bool sa, bool su);
	private:
		//string songFile;
		int timeOngoing;
		bool ongoing;
		
		const int alarmTime;	
		const bool mon;
		const bool tue;
		const bool wed;
		const bool thu;
		const bool fri;
		const bool sat;
		const bool sun;
};
class Alarm_Node{
	public:
		Alarm_Node(const Alarm a, Alarm_Node* anext) {};
		int setNext(Alarm_Node* aNext);
		const Alarm giveAlarm();
		Alarm_Node* giveNext();

	private:
		const Alarm alarm;
		Alarm_Node* next;
};	
class Alarm_LL{
	public:
		int Alarm_LL();
		int addAlarm(Alarm a);
		int delAlarm(int pos);
		int readList();
		int writeList();
		int displayList();
		
	private:
		Alarm_Node* head;
		int length;
};
#endif