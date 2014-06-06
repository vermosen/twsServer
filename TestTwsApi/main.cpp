/* Copyright (C) 2013 Interactive Brokers LLC. All rights reserved. This code is subject to the terms
* and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */

#include "contract.h"
#include <thOth/time/DateTime.hpp>

#ifdef _WIN32
# include <windows.h>
# define sleep( seconds) Sleep( seconds * 1000);
#else
# include <unistd.h>
#endif

#include "historicalRequestClient/historicalRequestClient.hpp"

const unsigned MAX_ATTEMPTS = 1;
const unsigned SLEEP_TIME = 10;

int main(int argc, char** argv) {

	try {
	
		const char* host = argc > 1 ? argv[1] : "";
		unsigned int port = argc > 2 ? atoi(argv[2]) : 7496;
		
		int clientId = 0;											// request Id
		unsigned attempt = 0;

		std::cout
			<< "Start of POSIX Socket Client Test "
			<< attempt
			<< std::endl;

		IB::Contract contract;										// contract to request
		contract.symbol = "MSFT";
		contract.secType = "STK";
		contract.exchange = "SMART";
		contract.currency = "USD";
		contract.primaryExchange = "NASDAQ";

		thOth::dateTime dt(2014, 6, 3);								// the date requested

		for (;;) {
			++attempt;
			
			std::cout
				<< "Attempt "
				<< attempt
				<< " of "
				<< MAX_ATTEMPTS
				<< std::endl;

			IB::historicalRequestClient client(contract, dt);		// creates the client

			client.connect(host, port, clientId);

			while (client.isConnected()) client.processMessages();

			if (attempt >= MAX_ATTEMPTS) break;

			std::cout
				<< "Sleeping "
				<< SLEEP_TIME
				<< " seconds before next attempt"
				<< std::endl;

			sleep(SLEEP_TIME);

		}
	
	}
	catch (std::exception & e) {
	
		std::cout
			<< "an error occured: "
			<< std::endl
			<< e.what()
			<< std::endl;
	
	}
	catch (...) {
	
		std::cout
			<< "an unknown error occured..."
			<< std::endl;

	}
	
	std::cout 
		<< "End of POSIX Socket Client Test\n"
		<< std::endl;

	system("pause");

}

