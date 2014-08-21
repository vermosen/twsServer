#include "functions/historicalRequest/historicalRequest.hpp"

void historicalRequest() {

	// step 1: initialization
	std::cout
		<< "historical Request Test"
		<< std::endl
		<< "-----------------------"
		<< std::endl
		<< std::endl
		<< "please provide some contract code : "
		<< std::endl;

	boost::timer tt;											// timer
	int clientId = 0; unsigned attempt = 0;						// request Id

	std::string contractCode; std::cin >> contractCode;			// provided code

	TWS_LOG(													// log
		std::string("contract provided: ")
			.append(contractCode))

	MYSQL * connect = mysql_init(NULL);							// initialize mySQL connection

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

	// recordset`& query
	IB::dataBase::tableContractRecordset contractRs(connect);	// table contract recordset

	std::string query(											// query to run
		"SELECT * FROM table_contract WHERE contract_symbol = '");
		query.append(contractCode);
		query.append("'");

	TWS_LOG(std::string("running query: ")						// log
		.append(query))

	if (!contractRs.select(query))								// query succeeded ?
		throw std::exception("instrument request failed");	
		
	query = "";													// for later usage

	if (contractRs.size() == 0)									// symbol found ?
		throw std::exception("symbol not found");

	if (contractRs.size() > 1)									// unique ?
		throw std::exception("different symbols found");

	IB::dataBase::recordId id = contractRs.cbegin()->first;		// contract Id
	IB::ContractDetails contract = contractRs.cbegin()->second;	// contract
	
	TWS_LOG(													// log
		std::string("contract details: ")
			.append(contract.summary.symbol)
			.append(", ")
			.append(contract.summary.secType)
			.append(", ")
			.append(contract.summary.exchange)
			.append(", ")
			.append(contract.summary.currency)
			.append(", ")
			.append(contract.summary.primaryExchange));

	// step 2: date of the request
	std::cout << "Please provide a request date (MM/dd/yyyy):"	// message
			  << std::endl;

	std::string dtStr; std::cin >> dtStr;
	std::shared_ptr<thOth::dateTime> requestDate;				// the date requested

	// TODO: overload the >> operator for datetime
	bool passed = false; while (!passed) {						// try to perform lexical_cast
	
		try {

			thOth::dateTime::Days   ds = 
				boost::lexical_cast<int>(
					dtStr.substr(3, 2));

			thOth::dateTime::Months mt = 
				boost::lexical_cast<int>(
					dtStr.substr(0, 2));

			thOth::dateTime::Years  yr = 
				boost::lexical_cast<unsigned short>(
					dtStr.substr(6, 4));
			
			requestDate =										// the date requested
				std::shared_ptr<thOth::dateTime>(
					new thOth::dateTime(yr, mt, ds));		

			passed = true;										// no exception raised
		
		} catch (boost::bad_lexical_cast & ex) {

			TWS_LOG(std::string("bad lexical cast exception")
				.append(ex.what()))

		} catch (std::exception & ex) {
		
			TWS_LOG(std::string("an error occured in historical Request function: ")
				.append(ex.what()))

			throw std::exception(ex.what());
		
		} catch (...) {

			TWS_LOG(std::string("an unknown error occured in historicaRequest function"))
			throw std::exception("an unknown error occured..."); 
		
		};
	
	};

	// step 3: download data
	TWS_LOG(std::string("requesting IB data for date: ")		// log
		.append(boost::lexical_cast<std::string>(*requestDate)))

	contract.summary.exchange = "SMART";						// setting exchange to SMART

	IB::historicalRequestClient client(							// creates the client				
		contract.summary,										// contract 
		*requestDate,											// startDate of the request
		IB::barSize::thirtySeconds,								// minimum bar size
		1, IB::dataDuration::day,								// period length and type
		IB::dataType::trade);									// data type

	thOth::timeSeries<IB::historicalQuoteDetails> ts;			// time series

	TWS_LOG("connecting to the server")							// log

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
					.append(boost::lexical_cast<std::string>(attempt))
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

	// step 4: check for previous import
	// strategy: check on the contract Id * date * exchange
	thOth::dateTime first = ts.begin()->first;					// first date
	thOth::dateTime last = ts.rbegin()->first;					// last date
	IB::dataBase::tableHistoricalBarRecordset barRs(connect);	// table contract recordset

	query.append(												// query to run
		"SELECT * FROM table_historical_bar WHERE (contract_ID = '")
		.append(boost::lexical_cast<std::string>(id))
		.append("' AND exchange = '")
		.append(contract.summary.exchange)
		.append("' AND bar_start > ");

	TWS_LOG(std::string("running query: ")						// log
		.append(query))

	if (!barRs.select(query))									// query succeeded ?
		throw std::exception("instrument request failed");

	if (barRs.size() > 0)										// symbol found ?
		throw std::exception("data already found,aborting import");

	if (IB::settings::instance().verbosity() > 0)				// verbose
		TWS_LOG(std::string("writing database"))

	for (thOth::timeSeries<IB::historicalQuoteDetails>::const_iterator
		It = ts.cbegin(); It != ts.cend(); It++) {
		
	if (IB::settings::instance().verbosity() > 1)				//verbose

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

	if (IB::settings::instance().verbosity() > 0)				// verbose
		TWS_LOG(												// log
			std::string("historical data download test completed in ")
				.append(boost::lexical_cast<std::string>(tt.elapsed()))
				.append(" seconds"))

};