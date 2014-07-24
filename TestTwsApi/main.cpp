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

// tws api test
#include "functions/historicalRequest/historicalRequest.hpp"
#include "functions/staticDataRequest/staticDataRequest.hpp"
#include "utilities/settings.hpp"
#include "utilities/define.hpp"

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

		// test generator
		std::cout << IB::settings::instance().generator().next() << std::endl;
		std::cout << IB::settings::instance().generator().next() << std::endl;
		std::cout << IB::settings::instance().generator().next() << std::endl;
		std::cout << IB::settings::instance().generator().next() << std::endl;
		std::cout << IB::settings::instance().generator().next() << std::endl;

		// end test
		TWS_LOG(std::string("starting TwsApiTest"))				// log

		for (int i = 1; i < argc; i++) {							// deals with optional arguments
			
			std::string arg(argv[i]);								// current std::string

			if (arg.substr(1, 7) == "verbose") {					// expects -verbose=n
				
				std::string str(arg.substr(9, arg.length() - 9));	// the value

				IB::settings::instance().verbosity(					// set the verbosity
					boost::lexical_cast<int>(str));

				TWS_LOG(std::string("sets verbosity to ")			// log
					.append(str))

			};

			if (arg.substr(1, 4) == "host") {						// expects -host=xxx.xx.x.xxx

				std::string str(arg.substr(6, arg.length() - 6));	// the value

				IB::settings::instance().ibHost(str);				// set the host

				TWS_LOG(std::string("sets host to ")			// log
					.append(str))

			};

			if (arg.substr(1, 4) == "port") {						// expects -port=xxxx

				std::string str(arg.substr(6, arg.length() - 6));	// the value

				IB::settings::instance().ibPort(					// set the port
					boost::lexical_cast<int>(str));

				TWS_LOG(std::string("sets port to ")				// log
					.append(str))

			};

			if (arg.substr(1, 3) == "log") {						// expects -log=C:/bla

				std::string str(arg.substr(5, arg.length() - 5));	// the value

				IB::settings::instance().logPath(str);				// set the log path

				TWS_LOG(std::string("sets log path to ")			// log
					.append(str))

			};

			if (arg.substr(1, 4) == "test") {						// expects -test=n
			
				std::string str(arg.substr(6, arg.length() - 6));	// the value

				test = boost::lexical_cast<int>(str);				// runs the selected test automatically
				
				end = true;											// for later use: only one attempt

				TWS_LOG(std::string("enforcing test ")				// log
					.append(str))

			}

		}

		// manual choice
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
			
				TWS_LOG(std::string("manual selection ")			// log
					.append(boost::lexical_cast<std::string>(res)))

			}
			
			switch (res) {											// switch over the tests available
			
				case 1:
				
					TWS_LOG(										// log
						std::string("starting historicalRequest test"))	

					historicalRequest();							// launch historical request process
					break;				

				case 2:

					TWS_LOG(										// log
						std::string("starting staticDataRequest test"))

					staticDataRequest();							// launch static data request process
					break;

				case 0:

					TWS_LOG(										// log
						std::string("manual exit"))

					end = true;										// stop the server
					break;

				default:											// unknown, invalid

					TWS_LOG(										// log
						std::string("invalid selection"))

					std::cout
						<< "invalid selection, please try again"
						<< std::endl;

					break;
			
			}
		
		} while (end == false);										// loop until exit

	} catch (std::exception & e) {									// exception management
			
		TWS_LOG(													// log
			std::string("an error occured: ")
				.append(e.what()))

		TWS_LOG(													// log
			std::string("exiting with code 1"))

		return 1;

	} catch (...) {													// unknown error
	
		TWS_LOG(													// log
			std::string("an unknown error occured"))

		TWS_LOG(													// log
			std::string("exiting with code 1"))

		return 1;

	}

	if (IB::settings::instance().verbosity() > 0) {					// exit

		TWS_LOG(													// log
			std::string("end of TwsApiTest"))

		system("pause");

	}
}

