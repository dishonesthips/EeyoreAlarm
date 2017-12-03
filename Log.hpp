#ifndef LOG
#define LOG
#include <string>

class Log {
	public:
		Log();
		void log(std::string, std::string);
		
	private:
		std::string filename;
};
#endif
