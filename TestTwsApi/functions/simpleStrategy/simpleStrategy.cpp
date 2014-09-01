#include "functions/simpleStrategy/simpleStrategy.hpp"

// this procedure implements a simple strategy
// and computes a few key figures
void simpleStrategy(const std::string & opt) {

	// step 1: initialization
	std::cout
		<< "Simple strategy test"
		<< std::endl
		<< "--------------------"
		<< std::endl
		<< std::endl;

	boost::timer tt;											// set a timer

	std::string contractCode; if (opt.empty()) {				// optionally provided

		std::cout
			<< "please provide some contract code : "
			<< std::endl;

		std::cin >> contractCode;								// user

	}
	else {

		contractCode = opt;

	}

	TWS_LOG_V(std::string("contract code provided: ")			// log
		.append(contractCode), 0)

	// step 2: checks if the contract is already in the table
	MYSQL * connect = mysql_init(NULL);							// connection

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

	if (!connect) throw std::exception("unable to reach mySQL database");

	IB::dataBase::tableContractRecordset rs(connect);			// the recordset object

	// the query
	std::string selectQuery("SELECT * FROM \
		table_contract WHERE contract_symbol = ");
	INSERT_SQL_STR(selectQuery, contractCode)

	TWS_LOG_V(std::string("running query: ")					// log
		.append(selectQuery), 0)

	// step 3: run the static data request
	if (!rs.selectQ(selectQuery))								// returns value ?
		TWS_LOG_V(std::string("unknown contract provided")		// log
			.append(selectQuery), 0)


	//if () {
	//	
	//	
	//	}
	//} else {
	//	
	//	contract = rs.begin

	//	
	//}

	//TWS_LOG_V(												// log
	//	std::string("instrument not in the database"), 0)
	//	IB::Contract contract;									// contract to request

	//contract.symbol = contractCode.c_str();		// attempt with contract default values
	//contract.currency = "USD";
	//contract.exchange = "SMART";
	//contract.primaryExchange = "NYSE";
	//contract.secType = "STK";

	//IB::staticDataRequestClient client(contract);			// request the contract 

	//TWS_LOG_V(std::string("connecting to tws server"), 0)	// log

	//	for (;;) {												// loop over attemps

	//	++attempt;

	//	if (IB::settings::instance().verbosity() > 0)

	//		TWS_LOG_V(std::string("attempt number ")		// log
	//		.append(boost::lexical_cast<std::string>(attempt))
	//		.append(" out of ")
	//		.append(boost::lexical_cast<std::string>(MAX_ATTEMPT)), 0)

	//		client.connect(
	//		IB::settings::instance().ibHost().c_str(),
	//		IB::settings::instance().ibPort());				// TODO: test if we have to provide connection id here

	//	while (client.isConnected()) client.processMessages();

	//	if (attempt > MAX_ATTEMPT)							// max attemps reached

	//		throw std::exception("failed to connect after max attempts");

	//	if (client.endOfData()) {							// download succedded

	//		TWS_LOG_V(std::string("download successful"), 0)// log	
	//			break;

	//	}
	//	else {

	//		TWS_LOG_V(std::string("sleeping ")				// log
	//			.append(boost::lexical_cast<std::string>(SLEEP_TIME))
	//			.append(" seconds before next attempt"), 0)

	//			boost::this_thread::sleep_for(					// sleep for 100 ms
	//			boost::chrono::milliseconds(SLEEP_TIME));

	//	}

	//	}														// end of for loop

	//TWS_LOG_V(												// partially log the contract details
	//	std::string("contract details: (symbol) ")
	//	.append(client.contract().symbol)
	//	.append(", (secType) ")
	//	.append(client.contract().secType)
	//	.append(", (currency) ")
	//	.append(client.contract().currency)
	//	.append(", (primary exchange) ")
	//	.append(client.contract().primaryExchange)
	//	.append(", (bondType) ")
	//	.append(client.contractDetails().bondType)
	//	.append(", (callable) ")
	//	.append(boost::lexical_cast<std::string>(client.contractDetails().callable))
	//	.append(", (category) ")
	//	.append(client.contractDetails().category)
	//	.append(", (contractMonth) ")
	//	.append(client.contractDetails().contractMonth)
	//	.append("..."), 1)

	//	// insert into the database
	//	TWS_LOG_V(													// log
	//	std::string("attempt to insert contract details"), 0)

	//	if (!rs.insert(client.contractDetails()))				// tries to insert 

	//		TWS_LOG_V(std::string("insert failed"), 0)

	//}
	//else {

	//	throw std::exception("contract already in the database");

	//}

	TWS_LOG_V(														// log
		std::string("simple strategy test completed in ")
		.append(boost::lexical_cast<std::string>(tt.elapsed()))
		.append(" seconds"), 0)

};