/* Copyright (C) 2013 Interactive Brokers LLC. All rights reserved. This code is subject to the terms
* and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */

// database setup
#define SERVER   "macbookwin"
#define USER     "test_user"
#define PASSWORD "test01"
#define DATABASE "tws_server"
#define PORT     3308
#define LOGPATH  "C://Temp/"

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

#include "functions/historicalRequest/historicalRequest.hpp"
#include "functions/staticDataRequest/staticDataRequest.hpp"
#include "utilities/settings.hpp"

int main(int argc, char** argv) {

	try {

		static ObjectHandler::EnumTypeRegistry enumTypeRegistry;	// registry

		IB::utilities::registerAll();								// register the factories

		bool end = false; int test = 0;

		IB::settings::instance().verbosity(0       );				// default settings
		IB::settings::instance().server   (SERVER  );
		IB::settings::instance().user     (USER    );
		IB::settings::instance().password (PASSWORD);
		IB::settings::instance().dataBase (DATABASE);
		IB::settings::instance().port     (PORT    );
		IB::settings::instance().ibPort   (7496    );

		IB::settings::instance().logPath(							// log path
			std::string(LOGPATH)
			.append("TwsApiTest")
			.append("_")
			.append(boost::posix_time::to_iso_string(
			boost::posix_time::second_clock::local_time()))
			.append("_")
			.append(".csv"));

		for (int i = 1; i < argc; i++) {							// deals with optional arguments
			
			std::string arg(argv[i]);								// current std::string

			if (arg.substr(1, 7) == "verbose") {					// expects -verbose=n
				
				std::string str(arg.substr(9, arg.length() - 9));	// the value

				IB::settings::instance().verbosity(					// set the verbosity
					boost::lexical_cast<int>(str));

				IB::settings::instance().log()->push_back(
					std::string("sets verbosity to ")
						.append(str));
				
			};

			if (arg.substr(1, 4) == "host") {						// expects -host=xxx.xx.x.xxx

				std::string str(arg.substr(6, arg.length() - 6));

				IB::settings::instance().ibHost(str);				// set the host

				IB::settings::instance().log()->push_back(
					std::string("sets host to ")
					.append(str));

			};

			if (arg.substr(1, 4) == "port") {						// expects -port=xxxx

				std::string str(arg.substr(6, arg.length() - 6));

				IB::settings::instance().ibPort(					// set the port
					boost::lexical_cast<int>(str));

			};

			if (arg.substr(1, 3) == "log") {						// expects -log=C:/bla

				IB::settings::instance().logPath(					// set the log path
					arg.substr(5, arg.length() - 5));

			};

			if (arg.substr(1, 4) == "test") {						// expects -test=n
			
				test = boost::lexical_cast<int>(					// runs the selected test automatically
					arg.substr(6, arg.length() - 6));
				
				end = true;											// for later use: only one attempt

			}

		}

		// TODO: add the comments to the log file
		if (IB::settings::instance().verbosity() > 1)
			std::cout												// title
				<< "Starting POSIX Socket Client server"
				<< std::endl
				<< "-----------------------------------"
				<< std::endl
				<< std::endl;

		do {														// loop over the choices
		
			int res = test;											// for manual definition

			if (test == 0) {
			
				std::cout											// message
					<< "Please select an activity: "
					<< std::endl
					<< "1 - historical data request"
					<< std::endl
					<< "2 - static data test"
					<< std::endl
					<< "0 - exit"
					<< std::endl;

				std::cin >> res;									// user defined test
			
			}
			
			switch (res) {											// switch over the tests available
			
				case 1:
				
					historicalRequest();							// launch historical request process
					break;
				
				case 2:

					staticDataRequest();							// launch static data request process
					break;

				case 0:

					end = true;										// stop the server
					break;

				default:											// unknown, invalid

					std::cout
						<< "invalid selection, please try again"
						<< std::endl;

					break;
			
			}
		
		} while (end == false);										// loop until exit

	} catch (std::exception & e) {									// exception management

																	// first log

		if (IB::settings::instance().verbosity() > 0)				// if verbosity > 0
			
			std::cout												// TODO: replace by a log
				<< "an error occured: "
				<< std::endl
				<< e.what()
				<< std::endl;
	
	} catch (...) {
	
		if (IB::settings::instance().verbosity() > 0)
			std::cout
				<< "an unknown error occured..."
				<< std::endl;

	}
	
	std::cout 
		<< "End of TWS Api Test\n"
		<< std::endl;

	if (IB::settings::instance().verbosity() > 0)
		system("pause");

}

