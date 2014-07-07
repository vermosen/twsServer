#include "functions/historicalRequest/historicalRequest.hpp"

void historicalRequest() {

	std::cout
		<< "historical Request Test"
		<< std::endl
		<< "-----------------------"
		<< std::endl
		<< std::endl
		<< "please provide some contract code : "
		<< std::endl;

	int clientId = 0; unsigned attempt = 0;						// request Id

	std::string contractCode; std::cin >> contractCode;			// provided code

	TWS_LOG(													// log
		std::string("contract provided: ")
			.append(contractCode))

	MYSQL * connect;											// initialize mySQL connection
	connect = mysql_init(NULL);									// initialize the variable

	TWS_LOG(													// log
		std::string("requesting contract details for: ")
		.append(contractCode))

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

	// recordset
	IB::dataBase::tableContractRecordset rs(connect);			// table contract recordset

	std::string query(											// query to run
		"SELECT * FROM table_contract WHERE contract_symbol = '");
		query.append(contractCode);
		query.append("'");

		TWS_LOG(std::string("running query: ")					// log
			.append(query))

	if (!rs.select(query))										// query succeeded ?
		throw std::exception("instrument request failed");	
		
	if (rs.size() == 0)											// symbol found ?
		throw std::exception("symbol not found");

	IB::Contract contract;
	contract.symbol = contractCode.c_str();
	contract.secType = "test";
	contract.exchange = "SMART";								// default to smart
	contract.currency = "USD";
	contract.primaryExchange = "NYSE";

	TWS_LOG(													// log
		std::string("contract details: ")
			.append(contract.symbol)
			.append(", ")
			.append(contract.secType)
			.append(", ")
			.append(contract.exchange)
			.append(", ")
			.append(contract.currency)
			.append(", ")
			.append(contract.primaryExchange));

	thOth::dateTime dt(2014, 6, 3);								// the date requested

	TWS_LOG(std::string("requesting date: ")					// log
		.append(boost::lexical_cast<std::string>(dt)))

	TWS_LOG(std::string("requesting query: ")					// log
		.append(query))
			
	IB::historicalRequestClient client(							// creates the client				
		contract,												// contract 
		dt,														// startDate of the request
		IB::barSize::thirtySeconds,								// bar size
		1, IB::dataDuration::day,								// period length and type
		IB::dataType::trade);									// data type

	thOth::timeSeries<IB::historicalQuoteDetails> ts;			// time series

	TWS_LOG(std::string("connecting to the server"))			// log

	for (;;) {													// loop over attemps

		++attempt;

		if (IB::settings::instance().verbosity() > 0)			// verbose ?

			TWS_LOG(std::string("attempt number ")				// log
				.append(boost::lexical_cast<std::string>(attempt))
				.append(" out of ")
				.append(boost::lexical_cast<std::string>(MAX_ATTEMPT)))

		client.connect(											// client is connecting
			IB::settings::instance().ibHost().c_str(),
			IB::settings::instance().ibPort(), clientId);

		while (client.isConnected()) client.processMessages();

		if (attempt >= MAX_ATTEMPT)								// max attemps reached
			throw std::exception("failed to connect after max attempts");

		if (client.endOfHistoricalData()) {						// download succedded

			if (IB::settings::instance().verbosity() > 0)

				TWS_LOG(std::string("attempt number ")			// log
					.append("download successful. \
						Trying to store data in the database"))
					
			ts = client.timeSeries();

			break;

		} else {

			if (IB::settings::instance().verbosity() > 2)

				TWS_LOG(std::string("sleeping ")				// log
					.append(boost::lexical_cast<std::string>(SLEEP_TIME))
					.append(" seconds before next attempt "))

			sleep(SLEEP_TIME);

		}

	}

	if (IB::settings::instance().verbosity() > 0)				// verbose

		TWS_LOG(std::string("writing data file"))

	for (thOth::timeSeries<IB::historicalQuoteDetails>::const_iterator
		It = ts.cbegin(); It != ts.cend(); It++) {
		
		
		if (IB::settings::instance().verbosity() > 1)			//verbose

			TWS_LOG(std::string("new data: d: ")
				.append(boost::lexical_cast<std::string>(It->first))
				.append(", p: ")
				.append(boost::lexical_cast<std::string>(It->second.close_))
				.append(", h: ")
				.append(boost::lexical_cast<std::string>(It->second.high_))
				.append(", l: ")
				.append(boost::lexical_cast<std::string>(It->second.low_))
				.append(", v: ")
				.append(boost::lexical_cast<std::string>(It->second.volume_)))

	}

	if (IB::settings::instance().verbosity() > 0)					// verbose

		TWS_LOG(std::string("end of historical data request"))

};