/* Copyright (C) 2013 Interactive Brokers LLC. All rights reserved. This code is subject to the terms
* and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */

#include "contract.h"
#include "utilities/type/all.hpp"

#include <thOth/time/DateTime.hpp>
#include <utilities/factory/registerAll.hpp>

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

		// register process
		static ObjectHandler::EnumTypeRegistry enumTypeRegistry;
		/*static ObjectHandler::EnumClassRegistry enumClassRegistry;
		static ObjectHandler::EnumPairRegistry enumPairRegistry;*/

		IB::utilities::registerAll();
	
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

		IB::historicalRequestClient client(							// creates the client				
			contract,												// contract 
			dt,														// startDate of the request
			IB::barSize::oneSecond,									// bar size
			1,														// period length
			IB::dataDuration::week,								    // period type
			IB::dataType::trades);									// data type

		for (;;) {													// loop over attemps

			++attempt;
			
			std::cout
				<< "Attempt "
				<< attempt
				<< " of "
				<< MAX_ATTEMPTS
				<< std::endl;

			client.connect(host, port, clientId);

			while (client.isConnected()) client.processMessages();

			if (attempt >= MAX_ATTEMPTS) break;						// max attemps reached

			std::cout
				<< "Sleeping "
				<< SLEEP_TIME
				<< " seconds before next attempt"
				<< std::endl;

			sleep(SLEEP_TIME);

		}

		thOth::TimeSeries<IB::historicalQuoteDetails> ts = client.timeSeries();
	
		std::cout
			<< "data summary: "
			<< std::endl
			<< "--------------"
			<< std::endl;

		for (thOth::TimeSeries<IB::historicalQuoteDetails>::const_iterator It = ts.cbegin(); It != ts.cend(); It++) {
		
			std::cout
				<< It->first
				<< " p: "
				<< It->second.close_
				<< " h: "
				<< It->second.high_
				<< " l: "
				<< It->second.low_
				<< " v: "
				<< It->second.volume_
				<< std::endl;
		
		}

		// creating csv file


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
		<< "End of TWS Api Test\n"
		<< std::endl;

	system("pause");

}

