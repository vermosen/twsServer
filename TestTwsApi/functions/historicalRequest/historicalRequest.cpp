#include "functions/historicalRequest/historicalRequest.hpp"

void historicalRequest(std::pair<IB::dataBase::recordId,IB::ContractDetails> contract_,
					   const thOth::dateTime & startDate_,
					   const thOth::dateTime & endDate_,
					   bool deletionPolicy_) {

	boost::timer tt;											// set a timer
	
	// step 1: initialization
	std::cout
		<< "historical Request Test"
		<< std::endl
		<< "-----------------------"
		<< std::endl
		<< std::endl;	

	TWS_LOG_V(													// log
		std::string("contract provided: ")
			.append(contract_.second.summary.symbol), 0)

	TWS_LOG_V(													// log
		std::string("contract details: ")
			.append(contract_.second.summary.symbol)
			.append(", ")
			.append(contract_.second.summary.secType)
			.append(", ")
			.append(contract_.second.summary.exchange)
			.append(", ")
			.append(contract_.second.summary.currency)
			.append(", ")
			.append(contract_.second.summary.primaryExchange), 0);

	MYSQL * connect = mysql_init(NULL);							// initialize mySQL connection

	TWS_LOG_V(													// log
		std::string("requesting contract details for: ")
		.append(contract_.second.summary.symbol), 0)

		if (!connect)											// fails to initialize mySQL
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

	IB::dataBase::tableHistoricalBarRecordset barRs(connect);	// bar recordset
	IB::ContractDetails ct = contract_.second;					// copy the contract_
	contract_.second.summary.exchange = "SMART";						// setting exchange to SMART
	thOth::dateTime dt = startDate_;								// date copy

	// step 3: loop over the dates
	do {
	
		singleHistoricalInsert(contract_, barRs,					// insert single contract_, do not throw
							   startDate_, 
							   deletionPolicy_);

		std::this_thread::sleep_for(							// sleep time necessary between two requests
			std::chrono::milliseconds(SLEEP_TIME_H));
	
		dt += boost::gregorian::days(1);						// add 1 day 
																// TODO: increment business days instead
	} while (dt <= endDate_);

	TWS_LOG_V(													// log
		std::string("historical data download test completed in ")
			.append(boost::lexical_cast<std::string>(tt.elapsed()))
			.append(" seconds."), 0)

};