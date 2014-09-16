#include "functions/bulkImport/bulkImport.hpp"

void bulkImport(const thOth::dateTime & startDate,
				const thOth::dateTime & endDate,
				bool deletionPolicy) {

	// step 0: variable intialization
	boost::timer tt;											// set a timer

	// step 1: initialization
	std::cout
		<< "bulk import test"
		<< std::endl
		<< "----------------"
		<< std::endl
		<< std::endl;

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

	if (!connect)												// fails to initialize the connection
		throw std::exception("unable to reach mySQL database");

	// recordset`& query
	IB::dataBase::tableContractRecordset contractRs(connect);	// table contract recordset

	std::string selectQuery(									// query to run
		"SELECT * FROM table_contract");

	TWS_LOG_V(std::string("running query: ")					// log
		.append(selectQuery), 0)

	if (!contractRs.selectQ(selectQuery))						// query succeeded ?
		throw std::exception("instrument request failed");

	if (contractRs.size() == 0)									// symbol found ?
		throw std::exception("no symbol found");

	TWS_LOG_V(													// log
		std::string("")
		.append(boost::lexical_cast<std::string>(contractRs.size()))
		.append(" contracts found"), 0);

	IB::dataBase::tableHistoricalBarRecordset barRs(connect);	// bar recordset

	// loop over the contracts
	for (std::map<IB::dataBase::recordId, IB::ContractDetails>::iterator It
		= contractRs.begin(); It != contractRs.end(); It++) {
	
		It->second.summary.exchange = "SMART";					// sets exchange to SMART

		thOth::dateTime dt = startDate;							// local copy

		// loop over the dates
		do {

			singleHistoricalBarInsert(
				*It, barRs, dt,									// insert single contract, do not throw
				deletionPolicy);

			std::this_thread::sleep_for(						// sleep time necessary between two requests
				std::chrono::milliseconds(SLEEP_TIME_H));

			dt += boost::gregorian::days(1);					// add 1 day 

		} while (dt <= endDate);
	}

	TWS_LOG_V(													// log
		std::string("bulk data download completed in ")
		.append(boost::lexical_cast<std::string>(tt.elapsed()))
		.append(" seconds."), 0)

};