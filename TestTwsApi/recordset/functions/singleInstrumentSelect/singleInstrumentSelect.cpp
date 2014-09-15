#include "recordset/functions/singleInstrumentSelect/singleInstrumentSelect.hpp"

std::pair<IB::dataBase::recordId, IB::ContractDetails> 
singleInstrumentSelect(const std::string & contractCode) {

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

	if (!connect) 
		throw std::exception("unable to reach mySQL database");

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

	if (contractRs.size() == 0)									// symbol found ?
		throw std::exception("symbol not found");

	if (contractRs.size() > 1)									// unique ?
		throw std::exception("different symbols found");

	return *contractRs.cbegin();

};