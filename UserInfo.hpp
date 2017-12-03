#ifndef USERINFO
#define USERINFO
#include <string>
#include "Log.hpp"

class UserInfo {
	public:
		const std::string filename = "userInfo.txt";
		UserInfo(Log log);	//declare constructor
		
		//declare functions
		bool fileNotExist();
		std::string getName();
		std::string getEmail();
		void writeInfo();
		void readInfo();
		
	private:
		//declare private variables
		std::string name;
		std::string email;
		Log logger;
		
		//error check methods do not require an instance of a class
		std::string capitalize(std::string name);
		static int checkName(const std::string input);
		static int checkEmail(const std::string input);
};
	
#endif
