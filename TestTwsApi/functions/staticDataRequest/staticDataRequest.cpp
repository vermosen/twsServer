#include "functions/staticDataRequest/staticDataRequest.hpp"

void staticDataRequest() {

	std::cout
		<< "please provide some contract code:"
		<< std::endl;

	int clientId = 0;											// request Id
	unsigned attempt = 0;

	std::string contractCode;
	std::cin >> contractCode;

	// initialize mySQL connection
	MYSQL * connect;											// connection
	connect = mysql_init(NULL);									// initialize the variable

	if (!connect)												// fails to initialize mySQL
		throw std::exception("mySQL initialization failed");

	connect = mysql_real_connect(								// mySQL real connection
		connect,
		IB::settings::instance().server  ().c_str(),
		IB::settings::instance().user    ().c_str(),
		IB::settings::instance().password().c_str(),
		IB::settings::instance().dataBase().c_str(),
		IB::settings::instance().port    (),
		NULL, 0);

	if (!connect) throw std::exception("unable to reach mySQL database");

	mysql_query(												// query to run
		connect,
		std::string("SELECT * FROM table_instrument WHERE instrument_symbol = '")
		.append(contractCode)
		.append("'").c_str());

	// mySQL query result
	MYSQL_RES * reception = mysql_store_result(connect);		// results

	if (!reception)												// request failed
		throw std::exception("instrument request failed");

	IB::Contract contract;										// contract to request

	if (mysql_num_rows(reception) != 1)							// assumed to be a unique record
		throw std::exception("unknown contract");

	MYSQL_ROW db_row = mysql_fetch_row(reception);

	contract.symbol = contractCode.c_str();
	contract.secType = db_row[2];
	contract.exchange = "SMART";								// default to smart
	contract.currency = db_row[3];
	contract.primaryExchange = db_row[4];

	thOth::dateTime dt(2014, 6, 3);								// the date requested

	IB::staticDataRequestClient client(							// creates the client				
		contract);												// contract 

	if (IB::settings::instance().verbosity() > 0)
		std::cout
			<< "connecting to the server..."
			<< std::endl;

	for (;;) {													// loop over attemps

		++attempt;

		if (IB::settings::instance().verbosity() > 0)
			std::cout
				<< std::endl
				<< "Attempt "
				<< attempt
				<< " out of "
				<< MAX_ATTEMPT_S
				<< std::endl;

		client.connect(
			IB::settings::instance().ibHost().c_str(),
			IB::settings::instance().ibPort(), clientId);

		while (client.isConnected()) client.processMessages();

		if (attempt >= MAX_ATTEMPT_S)							// max attemps reached
			throw std::exception("failed to connect after max attempts");

		if (client.endOfStaticData()) {							// download succedded

			if (IB::settings::instance().verbosity() > 0)
				std::cout
					<< "download successful..."
					<< std::endl
					<< "trying to store data "
					<< "in the database"
					<< std::endl;

			break;

		} else {

			if (IB::settings::instance().verbosity() > 2)
				std::cout
					<< "Sleeping "
					<< SLEEP_TIME_S
					<< " seconds before next attempt"
					<< std::endl;

			sleep(SLEEP_TIME_S);

		}

	}

	if (IB::settings::instance().verbosity() > 0)					// message
		std::cout 
			<< "static data download completed" 
			<< std::endl;

};