#include "functions/bulkImport/bulkImport.hpp"

void bulkImport(const std::string & opt1) {

	// step 1: initialization
	std::cout
		<< "bulk import Test"
		<< std::endl
		<< "----------------"
		<< std::endl
		<< std::endl;
		
	boost::timer tt;											// set a timer

	unsigned int attempt = 0;									// request Id	

	MYSQL * connect = mysql_init(NULL);							// initialize mySQL connection

	TWS_LOG_V(std::string("requesting contracts data"), 0) 		// log
	
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

	if (!connect)
		throw std::exception("unable to reach mySQL database");

	// recordset`& query
	IB::dataBase::tableContractRecordset contractRs(connect);	// table contract recordset

	std::string selectQuery(									// bulk query
		"SELECT * FROM table_contract");

	TWS_LOG_V(std::string("running query: ")					// log
		.append(selectQuery), 0)

	if (!contractRs.selectQ(selectQuery))						// query succeeded ?
		throw std::exception("instrument request failed");	
		
	selectQuery = "";											// reset for later usage

	if (contractRs.size() == 0)									// symbol found ?
		throw std::exception("symbol not found");
	
	TWS_LOG_V(boost::lexical_cast<std::string>(contractRs.size())													// log
		.append("contracts found in the database"), 0);

	// step 2: date of the request
	std::string dtStr; if (opt1.empty()) {				// optionally provided

		std::cout										// message
			<< "Please provide a request date (MM/dd/yyyy):"	
			<< std::endl;

		std::cin >> dtStr;								// user

	}
	else {

		dtStr = opt1;

	}

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

			TWS_LOG_V(std::string("bad lexical cast exception")
				.append(ex.what()), 0)

		} catch (std::exception & ex) {
		
			TWS_LOG_V(std::string("an error occured in historical Request function: ")
				.append(ex.what()), 0)

			throw std::exception(ex.what());
		
		} catch (...) {

			TWS_LOG_V(std::string("an unknown error occured in historicaRequest function"), 0)
			throw std::exception("an unknown error occured..."); 
		
		};
	
	};

	// step 3: download data
	TWS_LOG_V(std::string("requesting IB data for date: ")		// log
		.append(boost::lexical_cast<std::string>(*requestDate)), 0)

	std::vector<std::vector<thOth::bar> > bars;					// the bars

	for (std::map<IB::dataBase::recordId, IB::ContractDetails>::const_iterator It
		= contractRs.cbegin(); It != contractRs.cend(); It++) {

		//It->second.summary.exchange = IB::IBString("SMART");		// setting exchange to SMART

		IB::historicalRequestClient client(						// creates the client				
			It->second.summary,									// contract 
			*requestDate,										// startDate of the request
			IB::barSize::thirtySeconds,							// minimum bar size
			1, IB::dataDuration::day,							// period length and type
			IB::dataType::trade);								// data type
	

	}

	TWS_LOG_V("connecting to the server", 2)					// log

	//for (;;) {													// loop over attemps

	//	++attempt;

	//	TWS_LOG_V(std::string("attempt number ")				// log
	//		.append(boost::lexical_cast<std::string>(attempt))
	//		.append(" out of ")
	//		.append(boost::lexical_cast<std::string>(MAX_ATTEMPT)), 0)

	//	client.connect(											// client is connecting
	//		IB::settings::instance().ibHost().c_str(),
	//		IB::settings::instance().ibPort(), 
	//      IB::settings::instance().idGen().next());

	//	while (client.isConnected()) client.processMessages();	// TODO: turn into asynchronous

	//	if (attempt > MAX_ATTEMPT)								// max attemps reached
	//		throw std::exception("failed to connect after max attempts");

	//	if (client.endOfHistoricalData()) {						// download succedded

	//		TWS_LOG_V(std::string("attempt number ")			// log
	//			.append(boost::lexical_cast<std::string>(attempt))
	//			.append("download successful. \
	//				Trying to store data in the database"), 0)
	//				
	//		bars = client.bars();

	//		break;

	//	} else {

	//		TWS_LOG_V(std::string("sleeping ")					// log
	//			.append(boost::lexical_cast<std::string>(SLEEP_TIME))
	//			.append(" milli-seconds before next attempt "), 2)

	//		boost::this_thread::sleep_for(						// sleep for 100 ms
	//			boost::chrono::milliseconds(SLEEP_TIME));

	//	}

	//}

	//// step 4: check for previous import: contract Id * dateframe * exchange
	//IB::dataBase::tableHistoricalBarRecordset barRs(connect);	// table contract recordset

	//selectQuery.append("SELECT * FROM table_historical_bar WHERE (contract_ID = ");
	//	INSERT_SQL_NUM(selectQuery, id)
	//	selectQuery.append(" AND exchange = ");
	//	INSERT_SQL_STR(selectQuery, contract.summary.exchange)
	//	selectQuery.append(" AND bar_start >= ");
	//	INSERT_SQL_DATE(selectQuery, bars.begin()->barStart())
	//	selectQuery.append(" AND bar_end <= ");
	//	INSERT_SQL_DATE(selectQuery, bars.rbegin()->barEnd())
	//	selectQuery.append(")");

	//TWS_LOG_V(std::string("query to launch: ")					// log
	//	.append(selectQuery), 0)

	//try {

	//	if (barRs.selectQ(selectQuery) == true) {				// if succedded, there are item to delete
	//	
	//		TWS_LOG_V("previous import found, deleting data", 0)// deletion phase

	//		std::string deleteQuery;						// delete query
	//			deleteQuery.append("DELETE FROM TABLE_HISTORICAL_BAR WHERE (CONTRACT_ID = ");
	//			INSERT_SQL_NUM(deleteQuery, id)
	//			deleteQuery.append(" AND EXCHANGE = ");
	//			INSERT_SQL_STR(deleteQuery, contract.summary.exchange)
	//			deleteQuery.append(" AND BAR_START >= ");
	//			INSERT_SQL_DATE(deleteQuery, bars.begin()->barStart())
	//			deleteQuery.append(" AND BAR_END <= ");
	//			INSERT_SQL_DATE(deleteQuery, bars.rbegin()->barEnd())
	//			deleteQuery.append(")");

	//		barRs.deleteQ(deleteQuery);							// run the delete statement
	//	
	//	}
	//	
	//} catch (IB::dataBase::selectQueryExceptionNoSelection & ex) {
	//				
	//	// nothing to erase, go to the next step

	//} catch (IB::dataBase::selectQueryExceptionUnknownField & ex) {

	//	// problem with a field

	//} catch (std::exception & ex) {								// any other mistake -> throw
	//
	//	throw ex;				
	//
	//}

	//// step 5: insert new data to the table
	//TWS_LOG_V(std::string("writing database"), 0)

	//for (std::vector<thOth::bar>::const_iterator 
	//	It = bars.cbegin(); It != bars.cend(); It++) {

	//	barRs.insert(
	//		IB::dataBase::barRecord(
	//			id,
	//			*It,
	//			contract.summary.exchange));
	//	
	//	TWS_LOG_V(std::string("new data: d: ")
	//		.append(boost::lexical_cast<std::string>(It->barStart()))
	//		.append(", p: ")
	//		.append(boost::lexical_cast<std::string>(It->close()))
	//		.append(", h: ")
	//		.append(boost::lexical_cast<std::string>(It->high()))
	//		.append(", l: ")
	//		.append(boost::lexical_cast<std::string>(It->low()))
	//		.append(", v: ")
	//		.append(boost::lexical_cast<std::string>(It->volume())), 1)

	//}

	//TWS_LOG_V(												// log
	//	std::string("historical data download test completed in ")
	//		.append(boost::lexical_cast<std::string>(tt.elapsed()))
	//		.append(" seconds"), 0)

};