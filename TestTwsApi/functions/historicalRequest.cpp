#include "functions/historicalRequest.hpp"

void historicalRequest() {

	int verbosity = IB::settings::instance().verbosity();	// get the current settings


	std::cout
		<< "please provide some contract code:"
		<< std::endl;


	int clientId = 0;											// request Id
	unsigned attempt = 0;


	std::string contractCode;
	std::cin >> contractCode;

	// initialize mySQL connection
	MYSQL * connect;											// connection
	connect = mysql_init(NULL);									// initialize the variable


	if (!connect)												// fails to initialize mySQL
		throw std::exception("mySQL initialization failed");

	connect = mysql_real_connect(								// mySQL real connection
		connect,
		SERVER,
		USER,
		PASSWORD,
		DATABASE,
		PORT,
		NULL, 0);

	if (!connect) throw std::exception("unable to reach mySQL database");

	mysql_query(												// query to run
		connect,
		std::string("SELECT * FROM table_instrument WHERE instrument_symbol = '")
		.append(contractCode)
		.append("'").c_str());

	// mySQL query result
	MYSQL_RES * reception = mysql_store_result(connect);		// results

	if (!reception)												// request failed
		throw std::exception("instrument request failed");

	IB::Contract contract;										// contract to request

	if (mysql_num_rows(reception) != 1)							// assumed to be a unique record
		throw std::exception("unknown contract");

	MYSQL_ROW db_row = mysql_fetch_row(reception);

	contract.symbol = contractCode.c_str();
	contract.secType = db_row[2];
	contract.exchange = "SMART";								// default to smart
	contract.currency = db_row[3];
	contract.primaryExchange = db_row[4];

	thOth::dateTime dt(2014, 6, 3);								// the date requested

	IB::historicalRequestClient client(							// creates the client				
		contract,												// contract 
		dt,														// startDate of the request
		IB::barSize::thirtySeconds,								// bar size
		1,														// period length
		IB::dataDuration::day,								    // period type
		IB::dataType::trade);									// data type

	thOth::TimeSeries<IB::historicalQuoteDetails> ts;

	for (;;) {													// loop over attemps

		++attempt;

		std::cout
			<< "Attempt "
			<< attempt
			<< " out of "
			<< MAX_ATTEMPTS
			<< std::endl;

		client.connect(ibHost, ibPort, clientId);

		while (client.isConnected()) client.processMessages();

		if (attempt >= MAX_ATTEMPTS)							// max attemps reached
			throw std::exception("failed to connect after max attempts");

		if (client.endOfHistoricalData()) {						// download succedded

			std::cout
				<< "download successful, "
				<< "trying to store data "
				<< "in the database"
				<< std::endl;

			ts = client.timeSeries();
			break;

		}
		else {

			std::cout
				<< "Sleeping "
				<< SLEEP_TIME
				<< " seconds before next attempt"
				<< std::endl;

			sleep(SLEEP_TIME);

		}

	}

	//verbose
	for (thOth::TimeSeries<IB::historicalQuoteDetails>::const_iterator
		It = ts.cbegin(); It != ts.cend(); It++) {

		std::cout
			<< It->first
			<< " p: "
			<< It->second.close_
			<< " h: "
			<< It->second.high_
			<< " l: "
			<< It->second.low_
			<< " v: "
			<< It->second.volume_
			<< std::endl;

	}

	// creating csv file
	std::cout << "writing data file..." << std::endl;

	thOth::utilities::csvBuilder csv(								// csv path name
		std::string(LOG_PATH)										// todo: put this guy into settings
		.append(contract.symbol)
		.append("_")
		.append(boost::posix_time::to_iso_string(
		boost::posix_time::second_clock::local_time()))
		.append("_")
		.append(".csv"));

	csv.add("date_time", 1, 1);										// line headers
	csv.add("open", 1, 2);
	csv.add("close", 1, 3);
	csv.add("high", 1, 4);
	csv.add("low", 1, 5);
	csv.add("volume", 1, 6);

	long row = 2;
	for (thOth::TimeSeries<IB::historicalQuoteDetails>::const_iterator
		It = ts.cbegin(); It != ts.cend(); It++, row++) {

		csv.add(boost::lexical_cast<std::string>(It->first), row, 1);
		csv.add(It->second.open_, row, 2);
		csv.add(It->second.close_, row, 3);
		csv.add(It->second.high_, row, 4);
		csv.add(It->second.low_, row, 5);
		csv.add(It->second.volume_, row, 6);

	}

};