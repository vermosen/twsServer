#include "functions/simpleStrategy/simpleStrategy.hpp"

// this procedure implements a simple strategy
// and computes a few key figures
void simpleStrategy() {

	// step 1: initialization
	std::cout
		<< "simple strategy Test"
		<< std::endl
		<< "--------------------"
		<< std::endl
		<< std::endl
		<< "please provide some contract code : "
		<< std::endl;

	boost::timer tt;											// timer 

	int clientId = 0; unsigned attempt = 0;						// request Id

	std::string contractCode; std::cin >> contractCode;			// provided code

	TWS_LOG(													// log
		std::string("simple strategy test completed in ")
			.append(boost::lexical_cast<std::string>(tt.elapsed()))
			.append(" seconds."))

};