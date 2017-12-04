#ifndef READSTAT
#define READSTAT
#include <string>
#include "Log.hpp"

class ReadStat {
	public:
		static Log logger;

	
		ReadStat(const std::string);
		
		static void setLogger(Log);

		void readData();
		void specialDataLen(const int);
		void specialData(const int, const int*);
		void setMax();
		void setMin();
		void setMean();
		void sort();
		void setMedian();
		void histogram();
		bool fileExists();
		std::string* getStats();
		void displayStats();
		void writeStats(const std::string*, const std::string);
		int getLength();
		int* getData();
		const int bucketsLen = 8;
		
	private:
		std::string filename;
		int* data;
		int length;
		int max;
		int min;
		float mean;
		int median;
		int buckets[8];
};

#endif