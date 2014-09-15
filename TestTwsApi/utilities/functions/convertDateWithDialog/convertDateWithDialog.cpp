#include "utilities/functions/convertDateWithDialog/convertDateWithDialog.hpp"

// date conversion dialog
thOth::dateTime convertDateWithDialog(
	const std::string & dateName,
	std::string dtStr) {

	bool passed = false; while (!passed) {						// date management

		try {

			if (dtStr.empty()) {								// if string is empty, manual managment

				std::cout										// message
					<< "Please provide a "
					<< dateName
					<< " (MM/dd/yyyy):"
					<< std::endl;

				std::cin >> dtStr;								// user response

			}

			thOth::dateTime returnDate =						// trying to build the date from strng components
				thOth::dateTime(
				boost::lexical_cast<unsigned short>(dtStr.substr(6, 4)),
				boost::lexical_cast<int>(dtStr.substr(0, 2)),
				boost::lexical_cast<int>(dtStr.substr(3, 2)));

			passed = true;										// no exception raised

		}
		catch (boost::bad_lexical_cast & ex) {				// error 

			TWS_LOG_V(std::string("bad lexical cast exception: ")
				.append(ex.what()), 0)

				std::cout											// message
				<< dateName
				<< " conversion impossible, Please try again."
				<< std::endl;

			dtStr.clear();										// new attempt

		}
		catch (std::exception & ex) {

			TWS_LOG_V(std::string("an error occured in main: ")
				.append(ex.what()), 0)

				throw ex;

		}
		catch (...) {

			throw std::exception("an unknown error occured in main");

		}
	}
}