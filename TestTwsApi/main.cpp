/*
 * 
 * custom TwsApi
 * Copyright (C) 2014 Jean-Mathieu Vermosen
 *
 */

#define SERVER   "macbookwin"										// database setup
#define USER     "test_user"
#define PASSWORD "test01"
#define DATABASE "tws_server"
#define PORT     3308
#define LOGPATH  "C://Temp/"

#include <thOth/time/DateTime.hpp>									// thOth components
#include <utilities/factory/all.hpp>

#include "contract.h"												// TWS library
#include "functions/all.hpp"										// test functions
#include "utilities/type/all.hpp"
#include "utilities/settings/settings.hpp"							// additional 
#include "utilities/define.hpp"

// This procedure can be started manually (empty argv)
// or automatically by passing some argument through one
// of the following options:
//     - verbose: defines the level of verbosity
//     - host   : define the TWS server adress
//     - port   : define the TWS server port
//     - log    : overload the default log path
//     - test   : automatically start selected test
int main(int argc, char** argv) {

	try {

		static ObjectHandler::EnumTypeRegistry enumTypeRegistry;	// registry
		
		IB::utilities::registerAllFactories();						// register the factories
		
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

		TWS_LOG(std::string("starting TwsApiTest"))					// log

		std::string opt1, opt2;										// optional fields

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
				TWS_LOG(std::string("sets host to ")				// log
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
				TWS_LOG(std::string("enforcing test ")				// log
					.append(str))
				end = true;											// for later use: only one attempt

			}

			if (arg.substr(1, 4) == "opt1") {						// expects -opt1=toto

				std::string str(arg.substr(6, arg.length() - 6));	// the value
				opt1 = str;											// set the opt1 value
				TWS_LOG(std::string("sets opt1 value to ")			// log
					.append(str))

			}

			if (arg.substr(1, 4) == "opt2") {						// expects -opt1=toto

				std::string str(arg.substr(6, arg.length() - 6));	// the value
				opt2 = str;											// set the opt2 value
				TWS_LOG(std::string("sets opt2 value to ")			// log
					.append(str))

			}

		}
																	
		do {														// manual selection - loop over the choices
		
			int res = test;											// for manual definition

			if (test == 0) {
			
				std::cout											// message
					<< "Please select an activity: "
					<< std::endl
					<< "1 - static data test"
					<< std::endl
					<< "2 - historical data request"
					<< std::endl
					<< "3 - multi-threading test"
					<< std::endl
					<< "4 - csv writing test"
					<< std::endl
					<< "5 - debug test"
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
						std::string("starting staticDataRequest test"))

					staticDataRequest(opt1);						// launches static data request process
					break;

				case 2:
				
					TWS_LOG(										// log
						std::string("starting historicalRequest test"))	

					historicalRequest(opt1, opt2);					// launches historical request process
					break;				
				
				case 3:

					TWS_LOG(										// log
						std::string("starting multi-threaded settings test"))

						multiThreadedSetting();						// launches multi-threading
					break;

				case 4:

					TWS_LOG_V(										// log
						std::string("starting multi-threaded csv builder test"), 0)

						multiThreadedCsvBuilder();					// launches csv writing test
					break;

				case 5:

					TWS_LOG_V(										// log
						std::string("starting debug test"), 0)

						debug();									// launches csv writing test
					break;

				case 0:

					TWS_LOG_V(std::string("manual exit"), 0)		// log
					end = true;										// stop the server
					break;

				default:											// unknown, invalid

					TWS_LOG_V(										// log
						std::string("invalid selection"), 0)

					std::cout
						<< "invalid selection, please try again"
						<< std::endl;

					break;
			
			}
		
		} while (end == false);										// loop until exit

	} catch (std::exception & e) {									// exception management
			
		TWS_LOG_V(													// log
			std::string("an error occured: ")
				.append(e.what()), 0)

		TWS_LOG_V(std::string("exiting with code 1"), 0)					// log
		return 1;

	} catch (...) {													// unknown error
	
		TWS_LOG_V(std::string("an unknown error occured"), 0)			// log
		TWS_LOG_V(std::string("exiting with code 1"), 0)					// log
		return 1;

	}

	if (IB::settings::instance().verbosity() > 0) {					// exit

		TWS_LOG_V(std::string("end of TwsApiTest"), 0)
		system("pause");

	}
}

