#include "Mailer.hpp"
#include "UserInfo.hpp"
#include "Log.hpp"
#include <fstream>
#include <stdlib.h>
#include <string>

using namespace std;

//sets default values
/*Mailer::Mailer() {
	//initialise with blank info
	Log tmp = *(new Log());
	Mailer(new UserInfo(tmp), new Log());
}

//sets logger
Mailer::Mailer(const Log log) {
	Mailer(*(new UserInfo(log)), log);
}

//sets user details
Mailer::Mailer(const UserInfo info) {
	Mailer(info, new Log());
}*/

//sets user details and logger
Mailer::Mailer(UserInfo info, Log log) {
	user = &info;
	logger = log;
	sourceAddress = "eeyorealarm@gmail.com";
	smtpServer = "smtp.gmail.com";
	password = "onioneer";
	supportAddress = "adkarapo@edu.uwaterloo.ca";
}

//sends a statistics email
int Mailer::sendStats(const string statFile) {
	//set subject
	string subject = "Your Eeyore statistics";
	//body is already stored in file
	return sendMail(subject, statFile);
}

//sends a warning email
int Mailer::sendTimeout(const string alarmName, int timeoutLength) {
	string subject = "Timeout for alarm " + alarmName;
	string timeStr = to_string(timeoutLength);
	//save message body to file
	ofstream body;
	string filename = "timeout-mail";
	body.open(filename);
	body << "Hello, " << user->getName() << "!" << endl << endl <<
		"This is just to let you know that your alarm " << alarmName <<
		" has been running for " << timeoutLength <<
		" without being deactivated." << endl <<
		"We've turned the alarm off for you, just in case you aren't "
		<< "home. If you are home, you must be a really sound sleeper!"
		<< endl << "If this happens again, you may want to consider "
		<< "purchasing a more powerful alarm clock!" << endl << endl <<
		"\t- Your Eeyore alarm" << endl;
	body.close();
	//send mail
	int retVal = sendMail(subject, filename);
	//delete mail file
	if (remove(filename.c_str()) != 0) {
		//error deleting email
		logger.log("WARN", "Temporary email file could not be deleted");
		if (retVal != 0) {
			return retVal;
		} else {
			return -1;
		}
	}
	return retVal;
}

int Mailer::sendMail(const string subject, const string bodyFile) {
	//construct command string
	string command = "mailsend -to " + user->getName() +
		" -from " + sourceAddress + " -ssl -port 465 -auth-login " +
		"-smtp " + smtpServer + " -sub " + subject + " +cc +bc -v " +
		"-user " + sourceAddress + " -pass \"" + password + "\" " +
		"-name \"Eeyore Alarm\" -rt " + supportAddress +
		" -mime-type \"text/plain\" -msg-body \"" + bodyFile + "\"";
	//send mail
	return system(command.c_str());
}
