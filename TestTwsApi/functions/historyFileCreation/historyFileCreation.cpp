#include "functions/historyFileCreation/historyFileCreation.hpp"

// this procedure implements a simple strategy
// and computes a few key figures
void historyFileCreation(const std::string & opt) {

	// step 1: initialization
	std::cout
		<< "History File Creation"
		<< std::endl
		<< "---------------------"
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

	std::string instrumentQuery("SELECT * FROM \
		table_contract WHERE contract_symbol = ");				// the query
	INSERT_SQL_STR(instrumentQuery, contractCode)

	TWS_LOG_V(std::string("running query: ")					// log
		.append(instrumentQuery), 0)

	if (!rs.selectQ(instrumentQuery))							// query succeeded ?
		throw std::exception("instrument request failed");

	if (rs.size() == 0)											// symbol found ?
		throw std::exception("symbol not found");

	if (rs.size() > 1)											// unique ?
		throw std::exception("different symbols found");

	IB::dataBase::recordId id = rs.cbegin()->first;				// instrument id
	IB::Contract contract = rs.cbegin()->second.summary;			// contract to work with

	// step 3: load the historical data for the selected contract
	TWS_LOG_V(													// log
		std::string("instrument found, loading historical bars"), 2)
	
	IB::dataBase::tableHistoricalBarRecordset historicalRs(connect);

	std::string barQuery;										// for later usage (usefull ?)

	// TODO: try a JOIN statement
	barQuery.append("SELECT * FROM table_historical_bar WHERE (contract_ID = ");
		INSERT_SQL_NUM(barQuery, id)
		barQuery.append(")");

	std::vector<thOth::bar> bars;								// the bar vector

	try {
	
		if (historicalRs.selectQ(barQuery) == true) {			// query succeeded
		
			for (std::map<IB::dataBase::recordId, IB::dataBase::barRecord>::const_iterator It
				= historicalRs.cbegin(); It != historicalRs.cend(); It++)
			
				//bars.push_back(std::move(It->second.bar()));		// insert bars, move ?
				bars.push_back(It->second.bar());		// insert bars, move ?
		}	
	
	} catch (IB::dataBase::recordsetException & ex) {
	
		// aborting in all case
		throw ex;
	
	}
	
	// step 4: creates a csv file containing the data
	std::string csvPath(											// csv path
		std::string("C://Temp/")
			.append(opt)
			.append("_")
			.append("bars")
			.append(".csv"));

	thOth::utilities::csvBuilder csv(csvPath);						// csv file

	csv.add(std::string("bar start"), 1, 1);						// titles
	csv.add(std::string("bar end"  ), 1, 2);
	csv.add(std::string("open"     ), 1, 3);
	csv.add(std::string("close"    ), 1, 4);
	csv.add(std::string("high"     ), 1, 5);
	csv.add(std::string("low"      ), 1, 6);
	csv.add(std::string("volume"   ), 1, 7);

	long i = 2;	for (std::vector<thOth::bar>::const_iterator It		// insert data
		= bars.cbegin(); It != bars.cend(); It++; i++) {
	
		csv.add(boost::lexical_cast<std::string>(It->barStart()), i, 1);
		csv.add(boost::lexical_cast<std::string>(It->barEnd  ()), i, 2);
		
		csv.add(It->open  (), i, 3);
		csv.add(It->close (), i, 4);
		csv.add(It->high  (), i, 5);
		csv.add(It->low   (), i, 6);
		csv.add(It->volume(), i, 7);
		
	}
	
	TWS_LOG_V(														// log
		std::string("simple strategy test completed in ")
		.append(boost::lexical_cast<std::string>(tt.elapsed()))
		.append(" seconds"), 0)

};