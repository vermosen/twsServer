#include "functions/staticDataRequest/staticDataRequest.hpp"

void staticDataRequest() {

	std::cout
		<< "please provide some contract code:"
		<< std::endl;

	int clientId = 0;											// request Id
	unsigned attempt = 0;										// current attempt

	std::string contractCode; std::cin >> contractCode;			// contract is provided by the user

	TWS_LOG(std::string("contract code provided: ")				// log
		.append(contractCode))

	// check if the contract is already in the table
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

	std::string query("SELECT * FROM table_instrument WHERE instrument_symbol = '");
	query.append(contractCode)
		.append("'");
	
	mysql_query(												// query to run
		connect,
		query.c_str());

	TWS_LOG(std::string("running query: ")					// log
		.append(query))

	// mySQL query result
	MYSQL_RES * reception = mysql_store_result(connect);		// results

	if (!reception)												// request failed
		throw std::exception("instrument request failed");

	IB::Contract contract;										// contract to request

	if (mysql_num_rows(reception) == 1)							// if the contract is already in the db

		throw std::exception("contract already in the database");
	
	contract.symbol = contractCode.c_str();						// contract default caracteristics
	contract.currency = "USD";
	contract.exchange = "SMART";
	contract.primaryExchange = "NYSE";
	contract.secType = "STK";
	
	IB::staticDataRequestClient client(							// request the contrct details
		contract);

	if (IB::settings::instance().verbosity() > 0)

		TWS_LOG(std::string("connecting to tws server"))		// log

	for (;;) {													// loop over attemps

		++attempt;

		if (IB::settings::instance().verbosity() > 0)
			
			TWS_LOG(std::string("attempt number ")				// log
				.append(boost::lexical_cast<std::string>(attempt))
				.append(" out of ")
				.append(boost::lexical_cast<std::string>(MAX_ATTEMPT)))
			
		client.connect(
			IB::settings::instance().ibHost().c_str(),
			IB::settings::instance().ibPort(), clientId);

		while (client.isConnected()) client.processMessages();

		if (attempt >= MAX_ATTEMPT)								// max attemps reached

			throw std::exception("failed to connect after max attempts");

		if (client.endOfStaticData()) {							// download succedded

			if (IB::settings::instance().verbosity() > 0)

				TWS_LOG(std::string("download successful"))		// log
				
			break;

		} else {

			if (IB::settings::instance().verbosity() > 2)
				
				TWS_LOG(std::string("sleeping ")				// log
					.append(boost::lexical_cast<std::string>(SLEEP_TIME))
					.append(" seconds before next attempt"))

			sleep(SLEEP_TIME);

		}

	}

	// check for data
	if (IB::settings::instance().verbosity() > 0)

		TWS_LOG(												// log
			std::string("contract details: (symbol) ")
				.append(client.contract().symbol)
				.append(", (secType) ")
				.append(client.contract().secType)
				.append(", (currency) ")
				.append(client.contract().currency)
				.append(", (primary exchange) ")
				.append(client.contract().primaryExchange)
				.append(", (bondType) ")
				.append(client.contractDetails().bondType)
				.append(", (callable) ")
				.append(boost::lexical_cast<std::string>(client.contractDetails().callable))
				.append(", (category) ")
				.append(client.contractDetails().category)
				.append(", (contractMonth) ")
				.append(client.contractDetails().contractMonth))

	// insert into the database
	TWS_LOG(													// log
		std::string("attempt to insert contract details"))
	if (IB::settings::instance().verbosity() > 0)				// message
			
		TWS_LOG(												// log
			std::string("static data download test completed"))

};