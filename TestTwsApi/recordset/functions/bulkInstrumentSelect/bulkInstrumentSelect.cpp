#include "recordset/functions/bulkInstrumentSelect/bulkInstrumentSelect.hpp"

void bulkInstrumentSelect(IB::dataBase::tableContractRecordset::iterator & beg_,
					      IB::dataBase::tableContractRecordset::iterator & end_) {

	MYSQL * connect = mysql_init(NULL);							// initialize mySQL connection

	TWS_LOG_V(													// log
		std::string("bulk request of all the contracts"), 0)

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
	IB::dataBase::tableContractRecordset rs(connect);			// table contract recordset

	std::string selectQuery(									// query to run
		"SELECT * FROM table_contract");

	if (rs.selectQ(selectQuery)) {								// query succeeded ?
		
		beg_ = rs.begin();
		end_ = rs.end  ();
		
	}
};