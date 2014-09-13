#include "functions/debug/debug.hpp"

void debug() {

	// step 1: initialization
	std::cout
		<< "debug Test"
		<< std::endl
		<< "----------"
		<< std::endl;

	boost::timer tt;										// timer

	{
	
		// dateTime conversion into excel format
		thOth::dateTime test(2014, 1, 1, boost::posix_time::time_duration(0, 0, 15, 0));

		// print the excel float representation
		std::cout << boost::lexical_cast<std::string>(test.ExcelDate()) << std::endl;

	}

	TWS_LOG_V(												// log
		std::string("debug test completed in ")
			.append(boost::lexical_cast<std::string>(tt.elapsed()))
			.append(" seconds"), 0)

};