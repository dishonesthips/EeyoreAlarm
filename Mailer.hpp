#ifndef MAILER
#define MAILER
#include "Log.hpp"
#include <string>
class Mailer {
	public:
	
		static Log logger;

		Mailer(string name, string email);
		static void setLogger(Log log);

		int sendStats(const std::string statFile);
		int sendTimeout(const std::string alarmName, int timeoutLength);
		void updateInfo(string name, string email);
		


	private:
		int sendMail(const std::string subject, const std::string bodyFile);

		string userName;
		string userAddress;
		std::string sourceAddress;
		std::string smtpServer;
		std::string password;
		std::string supportAddress;
		
};
#endif
