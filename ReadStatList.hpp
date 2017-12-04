#ifndef READSTATLIST
#define READSTATLIST
#include "Log.hpp"
#include "ReadStat.hpp"
#include <string>


class ReadStatList {
	public:
		static Log logger;
		const int length = 10;
		const int lengthDays = 7;

		ReadStatList();
		static void setLogger(Log log);

		int runStats();

	private:
		ReadStat* stats[10];
};

#endif