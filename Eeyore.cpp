#include <stdlib.h>
#include <stdio.h>
#include <string>

using namespace std;

//class declarations
class Alarm_Node{
	public:
		Node() {};
		void setAlarm(int aData) { data = aData; };
		void SetNext(Node* aNext) { next = aNext; };
		int giveData() { return data; };
		Node* giveNext() { return next; };

	private:
		Alarm alarm;
		node *next;
};	
class Alarm{
	public:
	
	const static string DEFAULT_SONG_FILE;
	
	int alarmTime;
	int timeOngoing;
	bool ongoing;
	string songFile;
	
	
}
const string Alarm::DEFAULT_SONG_FILE = "DEFAULT_ALARM.wav";

  
