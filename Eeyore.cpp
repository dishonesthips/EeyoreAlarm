#include <iostream>

#include "Log.hpp"
#include "UserInfo.hpp"
#include "Alarm.hpp"
#include "AlarmList.hpp"

using namespace std;

int main() {
	Log logger;

	UserInfo user= UserInfo(logger);
	AlarmList alarmList = AlarmList(logger);
	alarmList.readList();

	
	bool exit = false;
	
	if (user.fileNotExist()) {
		cout<<"\n\tWelcome to Eeyore! Is this your first time?\n\tI don't recognize you...\n\n";
		user.writeInfo();
		cout<<"\n\n";
	}
	user.readInfo();
	//cout << user.getName() << " " << user.getEmail() << endl;
	cout<<"\n\t_____________________________\n\n\n\n";

	while (!exit){
		
		
		cout<<"\tWelcome to Eeyore, "<<user.getName()<<"\n\n\t"
			<<"1. Run Alarm System\n\t"
			<<"2. Add an Alarm\n\t"
			<<"3. Delete an Alarm\n\t"
			<<"4. View Alarms\n\t"
			<<"5. Update User Info\n\t"
			<<"6. View Statistics\n\t"
			<<"7. Exit\n\n\t"
			<<"Please enter the number corresponding to one of the options: ";

		string menuAnswer;
		getline(cin, menuAnswer);
		
		while (AlarmList::checkRange(menuAnswer,1,7)) {
			cout << "\tPlease enter a single digit in range [1,7]: ";
			getline(cin, menuAnswer);
		}

		if(menuAnswer[0] == '1'){//Run Alarm
			alarmList.runAlarm();
		}

		else if(menuAnswer[0] == '2'){//Add an Alarm
			cout<<"\n\tAdding an Alarm-\n";
			alarmList.addAlarm();
		}
		else if(menuAnswer[0] == '3'){//Delete an Alarm
			cout<<"\n\tDeleting an Alarm-\n";
			alarmList.displayList();
			alarmList.delAlarm();
		}
		else if (menuAnswer[0] == '4') {//View Alarms
			alarmList.displayList();
			cout<<"\n\tHit enter to continue... ";
			getline(cin,menuAnswer);
		}
		else if(menuAnswer[0] == '5'){//Update User Info
			cout<<"\n\tUpdating User Info-\n";
			user.writeInfo();
			user.readInfo();
		}
		else if(menuAnswer[0] == '6'){//View Statistics
			
		}
		else if(menuAnswer[0] == '7'){//Exit
			logger.log("TRACE","Manual request to exit program");
			exit = true;
		}
		else{
			logger.log("WARNING","Error checking menuAnswer went wrong, treated as if exit request;");
			exit = true;
		}

		cout<<"\n\n\n\t_____________________________\n\n\n\n";
	}
	cout<<"\tThanks for using Eeyore! Sweet Dreams!\n"<<endl;
	return 0;

}
