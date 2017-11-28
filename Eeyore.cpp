#include <stdlib.h>
#include <stdio.h>
#include <string>

using namespace std;

//class declarations
class Alarm{
	public:
	
		static string DEFAULT_SONG_FILE;
	
	private:
		string songFile;
		int alarmTime;
		int timeOngoing;
		bool ongoing;
	
	
};

class Alarm_Node{
	public:
		Alarm_Node() {};
		void setAlarm(int aData);
		void SetNext(Alarm_Node* aNext);
		int giveData();
		Alarm_Node* giveNext();

	private:
		Alarm alarm;
		Alarm_Node *next;
};	
class Alarm_LL{
			
	public:
		bool addAlarm(Alarm a);
		bool delAlarm(int pos);
		bool readList();
		//bool writeList();
		void displayList();
		
	private:
		Alarm_Node* head;
		int length;
};
int main(const int argc, const char* const argv[]) {
}
