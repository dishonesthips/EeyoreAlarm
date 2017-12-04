#include "Mailer.hpp"
#include "Log.hpp"
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

Log Mailer::logger;

Mailer::Mailer(string user, string email) {//sets user details and logger
	userName = user;
	userAddress = email;
	sourceAddress = "eeyorealarm@gmail.com";
	smtpServer = "smtp.gmail.com";
	password = "onioneer";
	supportAddress = "adkarapo@edu.uwaterloo.ca";
}


void Mailer::setLogger(Log log) {//logger
	Mailer::logger = log;
}


int Mailer::sendStats(const string statFile) {//sends a statistics email

	//set subject
	string subject = "Your Eeyore Statistics";
	//body is already stored in file
	return sendMail(subject, statFile);
}
int Mailer::sendTimeout(const string alarmName, int timeoutLength) {//sends a warning email
	string subject = "Timeout for alarm \"" + alarmName+"\"";
	string timeStr = to_string(timeoutLength);
	//save message body to file
	ofstream body;
	string filename = "timeout-mail";
	body.open(filename);
	body << "Hello, " << userName << "!" << endl << endl <<
		"This is just to let you know that your alarm " << alarmName <<
		" has been running for " << timeoutLength <<
		" seconds without being deactivated." << endl <<
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
void Mailer::updateInfo(string name, string email){
	userName = name;
	userAddress = email;
}


int Mailer::sendMail(const string subject, const string bodyFile) {
	//construct command string
	cout << "\n\tSending...check the email: " << userAddress << " soon."<< endl;

	string command = "mailsend -to \"" + userAddress +
		"\" -from \"" + sourceAddress + "\" -ssl -port 465 -auth-login " +
		"-smtp " + smtpServer + " -sub \"" + subject + "\" +cc +bc " +
		"-user " + sourceAddress + " -pass \"" + password + "\" " +
		"-name \"Eeyore Alarm\" -rt " + supportAddress +
		" -mime-type \"text/plain\" -msg-body \"" + bodyFile + "\" > /dev/null";
	//send mail
	logger.log("INFO","Sent email.");
	return system(command.c_str());
}
