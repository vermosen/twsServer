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

		// register the default settings
		IB::settings::instance().verbosity(0       );
		IB::settings::instance().server   (SERVER  );
		IB::settings::instance().user     (USER    );
		IB::settings::instance().password (PASSWORD);
		IB::settings::instance().dataBase (DATABASE);
		IB::settings::instance().port     (PORT    );
		IB::settings::instance().logPath  (LOGPATH );
		IB::settings::instance().ibPort   (7496    );

		// deals with optional arguments
		for (int i = 1; i < argc; i++) {							// check whether verbosity has been activated
			
			std::string arg(argv[i]);

			if (arg.substr(1, 7) == "verbose") {					// expected -verbose=n
				
				IB::settings::instance().verbosity(					// set the verbosity
					boost::lexical_cast<int>(arg.substr(9, arg.length() - 9)));
				
			};

			if (arg.substr(1, 4) == "host") {						// expected -host=xxx.xx.x.xxx

				IB::settings::instance().ibHost(					// set the host
					arg.substr(6, arg.length() - 6));

			};

			if (arg.substr(1, 4) == "port") {						// expected -port=xxxx

				IB::settings::instance().ibPort(					// set the host
					boost::lexical_cast<int>(arg.substr(6, arg.length() - 6)));

			};

		}

		if (IB::settings::instance().verbosity() > 1)
			std::cout												// title
				<< "Starting POSIX Socket Client server"
				<< std::endl
				<< "-----------------------------------"
				<< std::endl
				<< std::endl;

		// loop over the activities
		bool end = false; while (end == false) {
		
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
				
					historicalRequest();							// launch historical request process
					break;
				
				case 2:

					staticDataRequest();							// launch static data request process
					break;

				case 3:

					end = true;										// stop the server
					break;

				default:

					std::cout
						<< "invalid selection, please try again"
						<< std::endl;

					break;
			
			}
		
		}

	} catch (std::exception & e) {

		if (IB::settings::instance().verbosity() > 0)
			std::cout
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

