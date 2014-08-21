/*
 *
 * custom TwsApi
 * Copyright (C) 2014 Jean-Mathieu Vermosen
 *
 */

#include "utilities/settings/settings.hpp"

namespace IB {

	void settings::verbosity(const unsigned int v) {
		
		std::lock_guard<std::mutex> guard(verbosityMutex_);

		verbosity_ = v;
	
	}

	unsigned int settings::verbosity() const {
	
		std::lock_guard<std::mutex> guard(verbosityMutex_);

		return verbosity_; 
	
	}

	void settings::port(const unsigned int v) {

		std::lock_guard<std::mutex> guard(portMutex_);

		port_ = v;

	}

	unsigned int settings::port() const {

		std::lock_guard<std::mutex> guard(portMutex_);

		return port_;

	}

	void settings::ibPort(const unsigned int v) {

		std::lock_guard<std::mutex> guard(ibPortMutex_);

		ibPort_ = v;

	}

	unsigned int settings::ibPort() const {

		std::lock_guard<std::mutex> guard(ibPortMutex_);

		return ibPort_;

	}

	void settings::ibHost(const std::string & s) {

		std::lock_guard<std::mutex> guard(ibHostMutex_);

		ibHost_ = s;

	}

	std::string settings::ibHost() const {

		std::lock_guard<std::mutex> guard(ibHostMutex_);

		return ibHost_;

	}

	void settings::server(const std::string & s) {

		std::lock_guard<std::mutex> guard(serverMutex_);

		server_ = s;

	}

	std::string settings::server() const {

		std::lock_guard<std::mutex> guard(serverMutex_);

		return server_;

	}

	void settings::user(const std::string & s) {

		std::lock_guard<std::mutex> guard(userMutex_);

		user_ = s;

	}

	std::string settings::user() const {

		std::lock_guard<std::mutex> guard(userMutex_);

		return user_;

	}

	void settings::password(const std::string & s) {

		std::lock_guard<std::mutex> guard(passwordMutex_);

		password_ = s;

	}

	std::string settings::password() const {

		std::lock_guard<std::mutex> guard(passwordMutex_);

		return password_;

	}

	void settings::dataBase(const std::string & s) {

		std::lock_guard<std::mutex> guard(dataBaseMutex_);

		dataBase_ = s;

	}

	std::string settings::dataBase() const {

		std::lock_guard<std::mutex> guard(dataBaseMutex_);

		return dataBase_;

	}


}