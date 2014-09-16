#include "functions/staticDataRequest/staticDataRequest.hpp"

void staticDataRequest(const std::string & code,
					   bool deletionPolicy) {
	
	boost::timer tt;											// timer

	// step 1: initialization
	std::cout
		<< "static data request test"
		<< std::endl
		<< "------------------------"
		<< std::endl
		<< std::endl;

	TWS_LOG_V(std::string("contract code provided: ")			// log
		.append(code), 0)

	// step 1: prepare the contract
	IB::Contract ct;											// contract to request

	ct.symbol          = code   ;
	ct.exchange        = "SMART";
	ct.primaryExchange = "NYSE" ;
	ct.secType         = "STK"  ;

	IB::staticDataRequestClient client(ct);						// request the contract 

	TWS_LOG_V(std::string("connecting to tws server"), 0)		// log

	// step 2: connection attempt
	for (unsigned int attempt = 1;; attempt++) {				// loop over attemps

		if (IB::settings::instance().verbosity() > 0)

			TWS_LOG_V(std::string("attempt number ")			// log
				.append(boost::lexical_cast<std::string>(attempt))
				.append(" out of ")
				.append(boost::lexical_cast<std::string>(MAX_ATTEMPT)), 0)

			client.connect(
			IB::settings::instance().ibHost().c_str(),
			IB::settings::instance().ibPort());					// TODO: test if we have to provide connection id here

		while (client.isConnected()) client.processMessages();

		if (attempt > MAX_ATTEMPT)								// max attemps reached
			throw std::exception("failed to connect after max attempts");

		if (client.endOfData()) {								// download succedded

			TWS_LOG_V(std::string("download successful"), 0)	// log	
			break;

		} else {

			TWS_LOG_V(std::string("sleeping ")					// log
				.append(boost::lexical_cast<std::string>(SLEEP_TIME))
				.append(" seconds before next attempt"), 0)

			boost::this_thread::sleep_for(						// sleep for 100 ms
				boost::chrono::milliseconds(SLEEP_TIME));

		}

	}															// end of for loop

	TWS_LOG_V(													// partially log the contract details
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
		.append(client.contractDetails().contractMonth)
		.append("..."), 1)

	// step 3: insert into the database
	TWS_LOG_V(													// log
		std::string("attempt to insert contract details"), 0)

	MYSQL * connect = mysql_init(NULL);							// initialize mySQL connection

	if (!connect)												// fails to initialize mySQL
		throw std::exception("mySQL initialization failed");

	connect = mysql_real_connect(								// mySQL real connection
		connect,
		IB::settings::instance().server().c_str(),
		IB::settings::instance().user().c_str(),
		IB::settings::instance().password().c_str(),
		IB::settings::instance().dataBase().c_str(),
		IB::settings::instance().port(),
		NULL, 0);

	if (!connect)
		throw std::exception("unable to reach mySQL database");

	// recordset`& query
	IB::dataBase::tableContractRecordset rs(connect);			// table contract recordset

	if (!rs.insert(client.contractDetails()))					// tries to insert 
		TWS_LOG_V(std::string("insert failed"), 0)

	TWS_LOG_V(													// log
		std::string("static data download test completed in ")
			.append(boost::lexical_cast<std::string>(tt.elapsed()))
			.append(" seconds"), 0)

};