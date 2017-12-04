#ifndef MAILER
#define MAILER
#include "Log.hpp"
#include <string>
class Mailer {
	public:
		Mailer(string name, string email, Log log);
		int sendStats(const std::string statFile);
		int sendTimeout(const std::string alarmName, int timeoutLength);
	private:
		string userName;
		string userAddress;
		Log logger;
		std::string sourceAddress;
		std::string smtpServer;
		std::string password;
		std::string supportAddress;
		int sendMail(const std::string subject, const std::string bodyFile);
};
#endif
