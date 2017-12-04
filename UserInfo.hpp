#ifndef USERINFO
#define USERINFO
#include <string>
#include "Log.hpp"

class UserInfo {
	public:
		const std::string filename = "userInfo.txt";
		static Log logger;
		
		UserInfo();	
		static void setLogger(Log log);

		bool fileNotExist();
		
		
		std::string getName();
		std::string getEmail();
		
		
		void writeInfo();
		void readInfo();
		
		

	private:
		//error check methods do not require an instance of a class
		std::string capitalize(std::string name);
		static int checkName(const std::string input);
		static int checkEmail(const std::string input);
		
		//declare private variables
		std::string name;
		std::string email;
		
};
	
#endif
