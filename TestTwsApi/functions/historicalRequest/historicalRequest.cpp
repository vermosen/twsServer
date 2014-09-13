#include "functions/historicalRequest/historicalRequest.hpp"

void historicalRequest(bool deletionPolicy, 
					   const std::string & opt1,
					   const std::string & opt2,
					   const std::string & opt3) {

	// step 0: variable intialization
	boost::timer tt;											// set a timer
	
	// step 1: initialization
	std::cout
		<< "historical Request Test"
		<< std::endl
		<< "-----------------------"
		<< std::endl
		<< std::endl;
	
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

	if (!connect)									// fails to initialize mySQL
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
					<< "Please provide a request end date (MM/dd/yyyy):"
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

		} catch (std::exception & ex) {

			TWS_LOG_V(std::string("an error occured in historical Request function: ")
				.append(ex.what()), 0)

				throw ex;

		} catch (...) {

			throw std::exception("an unknown error occured in historicaRequest function");

		}
	} 

	IB::dataBase::tableHistoricalBarRecordset barRs(connect);	// bar recordset

	contract.summary.exchange = "SMART";						// setting exchange to SMART

	// step 3: loop over the dates
	do {
	
		singleHistoricalRequest(id, barRs,						// insert single contract, do not throw
								contract, 
								*requestStartDate, 
								deletionPolicy);

		std::this_thread::sleep_for(							// sleep time necessary between two requests
			std::chrono::milliseconds(SLEEP_TIME_H));
	
		*requestStartDate += boost::gregorian::days(1);			// add 1 day 
																// TODO: increment business days instead
	} while (*requestStartDate <= *requestEndDate);

	TWS_LOG_V(													// log
		std::string("historical data download test completed in ")
			.append(boost::lexical_cast<std::string>(tt.elapsed()))
			.append(" seconds."), 0)

};