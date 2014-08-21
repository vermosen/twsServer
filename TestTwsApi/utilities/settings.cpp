/*
 *
 * custom TwsApi
 * Copyright (C) 2014 Jean-Mathieu Vermosen
 *
 */

#include "utilities/settings.hpp"

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

}