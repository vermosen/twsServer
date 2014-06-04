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

int main(int argc, char** argv)
{
	const char* host = argc > 1 ? argv[1] : "";
	unsigned int port = argc > 2 ? atoi(argv[2]) : 7496;
	int clientId = 0;

	unsigned attempt = 0;
	printf("Start of POSIX Socket Client Test %u\n", attempt);

	// the contract
	IB::Contract contract;
	contract.symbol = "MSFT";
	contract.secType = "STK";
	contract.exchange = "SMART";
	contract.currency = "USD";

	// the date to request
	thOth::dateTime dt(2014, 6, 3);

	for (;;) {
		++attempt;
		printf("Attempt %u of %u\n", attempt, MAX_ATTEMPTS);

		IB::historicalRequestClient client(contract, dt);

		client.connect( host, port, clientId);

		while( client.isConnected()) {
		client.processMessages();
		}

		if( attempt >= MAX_ATTEMPTS) break;

		printf( "Sleeping %u seconds before next attempt\n", SLEEP_TIME);
		sleep( SLEEP_TIME);
	}

	printf("End of POSIX Socket Client Test\n");
}

