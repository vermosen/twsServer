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
#include "utilities/settings.hpp"

const unsigned MAX_ATTEMPTS = 2;
const unsigned SLEEP_TIME = 5;
const char * LOG_PATH = "C://Temp/";

int main(int argc, char** argv) {

	try {

		// register process
		static ObjectHandler::EnumTypeRegistry enumTypeRegistry;	// registry

		IB::utilities::registerAll();								// register the factories

		const char * ibHost = "";									// ip adress of the IB server
		unsigned int ibPort = 7496;									// remote port

		if (argc > 1) {												// some arguments added
		
			for (int i = 1; i < argc; i++) {					// check whether verbosity has been activated
			
				std::string arg(argv[i]);

				if (arg.substr(1, 7) == "verbose") {				// expected -verbose=n
				
					IB::settings::instance().verbosity(				// set the verbosity
						boost::lexical_cast<int>(arg.substr(9, 1)));
				
				};

				// TODO
				ibHost =
					(arg.substr(1, 4) == "host" ? "" : false);			// expected -host=xxx.xx.xx.xx
			
				ibPort =
					(arg.substr(1, 4) == "port" ? 7496 : false);		// expected -port=xxxx

			}

		}


		std::cout													// title
			<< "Starting POSIX Socket Client server"
			<< std::endl
			<< "-----------------------------------"
			<< std::endl
			<< std::endl;

		bool end = false;											// is test finished ?
		while (end == false) {										// main loop
		
			std::cout												// select the activity
				<< "Please select an activity: "
				<< std::endl
				<< "1 - historical data request"
				<< std::endl
				<< "2 - exit the server"
				<< std::endl;

			
			unsigned int res; std::cin >> res;						// user defined test

			switch (res) {
			
				case 1:
				
					break;

				case 2:

					end = true;										// stop the server
					break;

				default:

					std::cout
						<< "invalid test, please try again"
						<< std::endl;

					break;
			
			}

			//test sur la validité

			
		
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

