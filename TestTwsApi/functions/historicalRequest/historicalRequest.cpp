#include "functions/historicalRequest/historicalRequest.hpp"

void historicalRequest(const std::string & opt1,
					   const std::string & opt2,
					   const std::string & opt3) {

	// step 1: initialization
	std::cout
		<< "historical Request Test"
		<< std::endl
		<< "-----------------------"
		<< std::endl
		<< std::endl;
		
	boost::timer tt;											// set a timer

	unsigned int attempt = 0;									// number of attempts

	std::string contractCode; if (opt1.empty()) {				// optionally provided

		std::cout
			<< "please provide some contract code : "
			<< std::endl;

		std::cin >> contractCode;								// user

	} else {

		contractCode = opt1;
	
	}
	

	TWS_LOG_V(													// log
		std::string("contract provided: ")
			.append(contractCode), 0)

	MYSQL * connect = mysql_init(NULL);							// initialize mySQL connection

	TWS_LOG_V(													// log
		std::string("requesting contract details for: ")
		.append(contractCode), 0)

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

	std::string selectQuery(									// query to run
		"SELECT * FROM table_contract WHERE contract_symbol = '");
		selectQuery.append(contractCode);
		selectQuery.append("'");

	TWS_LOG_V(std::string("running query: ")					// log
		.append(selectQuery), 0)

	if (!contractRs.selectQ(selectQuery))						// query succeeded ?
		throw std::exception("instrument request failed");	
		
	selectQuery = "";											// for later usage

	if (contractRs.size() == 0)									// symbol found ?
		throw std::exception("symbol not found");

	if (contractRs.size() > 1)									// unique ?
		throw std::exception("different symbols found");

	IB::dataBase::recordId id = contractRs.cbegin()->first;		// contract Id
	IB::ContractDetails contract = contractRs.cbegin()->second;	// contract
	
	TWS_LOG_V(													// log
		std::string("contract details: ")
			.append(contract.summary.symbol)
			.append(", ")
			.append(contract.summary.secType)
			.append(", ")
			.append(contract.summary.exchange)
			.append(", ")
			.append(contract.summary.currency)
			.append(", ")
			.append(contract.summary.primaryExchange), 0);

	// step 2: dates of the request	
	std::shared_ptr<thOth::dateTime>							// the dates requested
		requestStartDate, requestEndDate;

	bool passed = false; while (!passed) {						// start date management
	
		try {
			
			std::string dtStr; if (opt2.empty()) {				// optionally provided

				std::cout										// message
					<< "Please provide a request start date (MM/dd/yyyy):"
					<< std::endl;

				std::cin >> dtStr;								// user

			} else {

				dtStr = opt2;

			}

			requestStartDate =									// the start date requested
				std::shared_ptr<thOth::dateTime>(
					new thOth::dateTime(
						boost::lexical_cast<unsigned short>(dtStr.substr(6, 4)), 
						boost::lexical_cast<int>(dtStr.substr(0, 2)), 
						boost::lexical_cast<int>(dtStr.substr(3, 2))));

			passed = true;										// no exception raised
		
		} catch (boost::bad_lexical_cast & ex) {				// error 

			TWS_LOG_V(std::string("bad lexical cast exception: ")
				.append(ex.what()), 0)

			if (!opt2.empty()) throw ex;						// function argument, no chance to recover

			std::cout											// message
				<< "Start date conversion impossible, Please try again."
				<< std::endl;

		} catch (std::exception & ex) {
		
			TWS_LOG_V(std::string("an error occured in historical Request function: ")
				.append(ex.what()), 0)

			throw ex;	
		
		} catch (...) {

			throw std::exception("an unknown error occured in historicaRequest function"); 
		
		}
	
	}

	passed = false; while (!passed) {							// try to perform lexical_cast

		try {

			std::string dtEnd; if (opt3.empty()) {				// optionally provided

				std::cout										// message
					<< "Please provide a request start date (MM/dd/yyyy):"
					<< std::endl;

				std::cin >> dtEnd;								// user

			}
			else {

				dtEnd = opt3;

			}

			requestEndDate =									// the start date requested
				std::shared_ptr<thOth::dateTime>(
				new thOth::dateTime(
				boost::lexical_cast<unsigned short>(dtEnd.substr(6, 4)),
				boost::lexical_cast<int>(           dtEnd.substr(0, 2)),
				boost::lexical_cast<int>(			dtEnd.substr(3, 2))));

			passed = true;										// no exception raised

		} catch (boost::bad_lexical_cast & ex) {				// error 

			TWS_LOG_V(std::string("bad lexical cast exception: ")
				.append(ex.what()), 0)

				if (!opt3.empty()) throw ex;					// function argument, no chance to recover

			std::cout											// message
				<< "End date conversion impossible, Please try again."
				<< std::endl;

		}
		catch (std::exception & ex) {

			TWS_LOG_V(std::string("an error occured in historical Request function: ")
				.append(ex.what()), 0)

				throw ex;

		} catch (...) {

			throw std::exception("an unknown error occured in historicaRequest function");

		}

	} 

	// step 3: download data
	// TODO: make a loop
	TWS_LOG_V(std::string("requesting IB data for date: ")		// log
		.append(boost::lexical_cast<std::string>(*requestStartDate)), 0)

	contract.summary.exchange = "SMART";						// setting exchange to SMART

	IB::historicalRequestClient client(							// creates the client				
		contract.summary,										// contract 
		*requestStartDate,										// startDate of the request
		IB::barSize::thirtySeconds,								// minimum bar size
		1, IB::dataDuration::day,								// period length and type
		IB::dataType::trade);									// data type

	std::vector<thOth::bar> bars;								// the bars

	TWS_LOG_V("connecting to the server", 2)					// log

	for (;;) {													// loop over attemps

		++attempt;

		TWS_LOG_V(std::string("attempt number ")				// log
			.append(boost::lexical_cast<std::string>(attempt))
			.append(" out of ")
			.append(boost::lexical_cast<std::string>(MAX_ATTEMPT)), 0)

		client.connect(											// client is connecting
			IB::settings::instance().ibHost().c_str(),
			IB::settings::instance().ibPort());

		while (client.isConnected()) client.processMessages();	// TODO: turn into asynchronous

		if (attempt > MAX_ATTEMPT)								// max attemps reached
			throw std::exception("failed to connect after max attempts");

		if (client.endOfData()) {								// download succedded

			TWS_LOG_V(std::string("attempt number ")			// log
				.append(boost::lexical_cast<std::string>(attempt))
				.append("download successful. \
					Trying to store data in the database"), 0)
					
			bars = client.bars();

			break;

		} else {

			TWS_LOG_V(std::string("sleeping ")					// log
				.append(boost::lexical_cast<std::string>(SLEEP_TIME))
				.append(" milli-seconds before next attempt "), 2)

			boost::this_thread::sleep_for(						// sleep for 100 ms
				boost::chrono::milliseconds(SLEEP_TIME));

		}

	}

	// step 4: check for previous import: contract Id * dateframe * exchange
	IB::dataBase::tableHistoricalBarRecordset barRs(connect);	// table contract recordset

	selectQuery.append("SELECT * FROM table_historical_bar WHERE (contract_ID = ");
		INSERT_SQL_NUM(selectQuery, id)
		selectQuery.append(" AND exchange = ");
		INSERT_SQL_STR(selectQuery, contract.summary.exchange)
		selectQuery.append(" AND bar_start >= ");
		INSERT_SQL_DATE(selectQuery, bars.begin()->barStart())
		selectQuery.append(" AND bar_end <= ");
		INSERT_SQL_DATE(selectQuery, bars.rbegin()->barEnd())
		selectQuery.append(")");

	TWS_LOG_V(std::string("query to launch: ")					// log
		.append(selectQuery), 0)

	try {

		if (barRs.selectQ(selectQuery) == true) {				// if succedded, there are item to delete
		
			TWS_LOG_V("previous import found, deleting data", 0)// deletion phase

			std::string deleteQuery;							// delete query
				deleteQuery.append("DELETE FROM TABLE_HISTORICAL_BAR WHERE (CONTRACT_ID = ");
				INSERT_SQL_NUM(deleteQuery, id)
				deleteQuery.append(" AND EXCHANGE = ");
				INSERT_SQL_STR(deleteQuery, contract.summary.exchange)
				deleteQuery.append(" AND BAR_START >= ");
				INSERT_SQL_DATE(deleteQuery, bars.begin()->barStart())
				deleteQuery.append(" AND BAR_END <= ");
				INSERT_SQL_DATE(deleteQuery, bars.rbegin()->barEnd())
				deleteQuery.append(")");

			barRs.deleteQ(deleteQuery);							// run the delete statement
		
		}
		
	} catch (IB::dataBase::selectQueryExceptionNoSelection & ex) {
																// nothing to erase, go to the next step
		TWS_LOG_V("selectQueryExceptionNoSelection raised, \
				   continuing current procedure", 2)

	} catch (IB::dataBase::selectQueryExceptionUnknownField & ex) {
																// problem with a field, throw
		throw ex;

	} catch (std::exception & ex) {								// any other mistake -> throw
	
		throw ex;				
	
	}

	// step 5: insert new data to the table
	TWS_LOG_V(std::string("writing database"), 0)

	for (std::vector<thOth::bar>::const_iterator 
		It = bars.cbegin(); It != bars.cend(); It++) {

		barRs.insert(
			IB::dataBase::barRecord(
				id,
				*It,
				contract.summary.exchange));
		
		TWS_LOG_V(std::string("new data: d: ")
			.append(boost::lexical_cast<std::string>(It->barStart()))
			.append(", p: ")
			.append(boost::lexical_cast<std::string>(It->close()))
			.append(", h: ")
			.append(boost::lexical_cast<std::string>(It->high()))
			.append(", l: ")
			.append(boost::lexical_cast<std::string>(It->low()))
			.append(", v: ")
			.append(boost::lexical_cast<std::string>(It->volume())), 1)

	}

	TWS_LOG_V(												// log
		std::string("historical data download test completed in ")
			.append(boost::lexical_cast<std::string>(tt.elapsed()))
			.append(" seconds"), 0)

};