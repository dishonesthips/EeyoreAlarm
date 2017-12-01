#include <iostream>
#include <fstream>
#include <stdlib.h>

using namespace std;

int main() {
	char name[] = "FOO";
	char time[] = "BAR";
	char elapsed[] = "BAZ";
	//create blank message
	ofstream outFile;
	outFile.open("mail.txt");
	outFile << "Hello, " << name << "!" << endl << endl;
	outFile << "This is just to let you know that the alarm you set for " << time << " today has been running for " << elapsed << " minutes without being deactivated." << endl;
	outFile << "If your alarm runs for too long, we will turn it off for you." << endl << endl;
	outFile << "- Your Eeyore alarm" << endl;
	outFile.close();
	//send message
	system("mailsend -to andrew.karapostolakis@gmail.com \
		-from eeyorealarm@gmail.com \
		-ssl -port 465 -auth-login \
		-smtp smtp.gmail.com \
		-sub \"Alarm notification\" \
		+cc +bc -v \
		-user eeyorealarm@gmail.com \
		-pass \"onioneer\" \
		-name \"Eeyore Alarm\" \
		-rt adkarapo@edu.uwaterloo.ca \
		-mime-type \"text/plain\" \
		-msg-body \"mail.txt\"");
	cout << "Done" << endl;
	return 0;
}
