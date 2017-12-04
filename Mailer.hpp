#ifndef MAILER
#define MAILER
#include <string>
#include "UserInfo.hpp"
#include "Log.hpp"

class Mailer {
	public:
		/*Mailer();
		Mailer(const Log log);
		Mailer(const UserInfo info);*/
		Mailer(UserInfo info, Log log);
		int sendStats(const std::string statFile);
		int sendTimeout(const std::string alarmName, int timeoutLength);
	private:
		UserInfo *user;
		Log logger;
		std::string sourceAddress;
		std::string smtpServer;
		std::string password;
		std::string supportAddress;
		int sendMail(const std::string subject, const std::string bodyFile);
};

#endif
