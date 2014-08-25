/*
 *
 * custom TwsApi
 * Copyright (C) 2014 Jean-Mathieu Vermosen
 *
 */

#include "utilities/settings/settings.hpp"

namespace IB {

	void settings::verbosity(const unsigned int v) {
		
		boost::lock_guard<boost::mutex> guard(verbosityMutex_);

		verbosity_ = v;
	
	}

	unsigned int settings::verbosity() const {
	
		boost::lock_guard<boost::mutex> guard(verbosityMutex_);

		return verbosity_; 
	
	}

	void settings::port(const unsigned int v) {

		boost::lock_guard<boost::mutex> guard(portMutex_);

		port_ = v;

	}

	unsigned int settings::port() const {

		boost::lock_guard<boost::mutex> guard(portMutex_);

		return port_;

	}

	void settings::ibPort(const unsigned int v) {

		boost::lock_guard<boost::mutex> guard(ibPortMutex_);

		ibPort_ = v;

	}

	unsigned int settings::ibPort() const {

		boost::lock_guard<boost::mutex> guard(ibPortMutex_);

		return ibPort_;

	}

	void settings::ibHost(const std::string & s) {

		boost::lock_guard<boost::mutex> guard(ibHostMutex_);

		ibHost_ = s;

	}

	std::string settings::ibHost() const {

		boost::lock_guard<boost::mutex> guard(ibHostMutex_);

		return ibHost_;

	}

	void settings::server(const std::string & s) {

		boost::lock_guard<boost::mutex> guard(serverMutex_);

		server_ = s;

	}

	std::string settings::server() const {

		boost::lock_guard<boost::mutex> guard(serverMutex_);

		return server_;

	}

	void settings::user(const std::string & s) {

		boost::lock_guard<boost::mutex> guard(userMutex_);

		user_ = s;

	}

	std::string settings::user() const {

		boost::lock_guard<boost::mutex> guard(userMutex_);

		return user_;

	}

	void settings::password(const std::string & s) {

		boost::lock_guard<boost::mutex> guard(passwordMutex_);

		password_ = s;

	}

	std::string settings::password() const {

		boost::lock_guard<boost::mutex> guard(passwordMutex_);

		return password_;

	}

	void settings::dataBase(const std::string & s) {

		boost::lock_guard<boost::mutex> guard(dataBaseMutex_);

		dataBase_ = s;

	}

	std::string settings::dataBase() const {

		boost::lock_guard<boost::mutex> guard(dataBaseMutex_);

		return dataBase_;

	}


}