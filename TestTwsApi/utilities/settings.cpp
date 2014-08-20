/*
 *
 * custom TwsApi
 * Copyright (C) 2014 Jean-Mathieu Vermosen
 *
 */

#include "utilities/settings.hpp"

namespace IB {

	idGenerator & settings::generator() {

		return generator_;

	}

	void settings::verbosity(const unsigned int v) {
		
		std::lock_guard<std::mutex> guard(verbosityMutex_);

		verbosity_ = v;
	
	}

	unsigned int settings::verbosity() const {
	
		std::lock_guard<std::mutex> guard(verbosityMutex_);

		return verbosity_; 
	
	}

}