#ifndef USERINFO
#define USERINFO
#include <string>

class UserInfo {
	public:
		const std::string filename = "userInfo.txt";
		UserInfo();	//declare constructor
		
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
		
		//error check methods do not require an instance of a class
		std::string capitalize(std::string name);
		static int checkName(const std::string input);
		static int checkEmail(const std::string input);
};
#endif
