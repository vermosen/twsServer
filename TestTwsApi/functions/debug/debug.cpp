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
	
		MYSQL * conn = IB::settings::instance().connection();

		IB::dataBase::tableContractRecordset2 rs(conn);

	}

	TWS_LOG_V(												// log
		std::string("debug test completed in ")
			.append(boost::lexical_cast<std::string>(tt.elapsed()))
			.append(" seconds"), 0)

};