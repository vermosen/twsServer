/* Copyright (C) 2013 Interactive Brokers LLC. All rights reserved. This code is subject to the terms
* and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */

// database setup
#define SERVER "macbookwin"
#define USER "test_user"
#define PASSWORD "test01"
#define DATABASE "tws_server"
#define PORT 3308

// TWS library
#include "contract.h"
#include "utilities/type/all.hpp"

// thOth components
#include <thOth/time/DateTime.hpp>
#include <thOth/utilities/csvBuilder.hpp>
#include <utilities/factory/registerAll.hpp>

// mySQL
#include <my_global.h>
#include <mysql.h>

#ifdef _WIN32
# include <windows.h>
# define sleep( seconds) Sleep( seconds * 1000);
#else
# include <unistd.h>
#endif

#include "historicalRequestClient/historicalRequestClient.hpp"

const unsigned MAX_ATTEMPTS = 2;
const unsigned SLEEP_TIME = 5;
const char * CSV_PATH = "C://Temp/";

int main(int argc, char** argv) {

	try {

		// register process
		static ObjectHandler::EnumTypeRegistry enumTypeRegistry;

		IB::utilities::registerAll();
	
		const char* host = argc > 1 ? argv[1] : "";
		unsigned int port = argc > 2 ? atoi(argv[2]) : 7496;

		int clientId = 0;											// request Id
		unsigned attempt = 0;

		std::cout													// message
			<< "Starting POSIX Socket Client Test"
			<< std::endl;

		IB::Contract contract;										// contract to request

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
			"SELECT * FROM table_instrument WHERE instrument_symbol = 'MSFT'");

		// mySQL query result
		MYSQL_RES * reception = mysql_store_result(connect);		// results

		if (!reception) 
			throw std::exception("no record found in table_instrument");

		if (mysql_num_rows(reception) == 1) {						// assumed to be a unique record
			
			MYSQL_ROW db_row = mysql_fetch_row(reception);

			contract.symbol = "MSFT";
			contract.secType = db_row[2];
			contract.exchange = "SMART";
			contract.currency = db_row[3];
			contract.primaryExchange = db_row[4];
		
		};			// number of rows

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
				<< " of "
				<< MAX_ATTEMPTS
				<< std::endl;

			client.connect(host, port, clientId);

			while (client.isConnected()) client.processMessages();

			if (attempt >= MAX_ATTEMPTS)							// max attemps reached
				throw std::exception("failed to connect after max attempts");						

			if (client.endOfHistoricalData()) {						// download succedded
			
				std::cout
					<< "download successful, "
					<< "trying to store results "
					<< "in the database"
					<< std::endl;

				ts = client.timeSeries();
				break;

			} else {			

				std::cout
					<< "Sleeping "
					<< SLEEP_TIME
					<< " seconds before next attempt"
					<< std::endl;

				sleep(SLEEP_TIME);
			
			}

		}

		//verbose
		for (thOth::TimeSeries<IB::historicalQuoteDetails>::const_iterator It = ts.cbegin(); It != ts.cend(); It++) {
		
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
			std::string(CSV_PATH)
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

	}
	catch (std::exception & e) {
	
		std::cout
			<< "an error occured: "
			<< std::endl
			<< e.what()
			<< std::endl;
	
	}
	catch (...) {
	
		std::cout
			<< "an unknown error occured..."
			<< std::endl;

	}
	
	std::cout 
		<< "End of TWS Api Test\n"
		<< std::endl;

	system("pause");

}

