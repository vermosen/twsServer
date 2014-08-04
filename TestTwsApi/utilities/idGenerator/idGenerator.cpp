/*
 *
 * custom TwsApi
 * Copyright (C) 2014 Jean-Mathieu Vermosen
 *
 */

#include "utilities/idGenerator/idGenerator.hpp"

namespace IB {

	idGenerator::idGenerator() : lastId_(0) {}

	TickerId idGenerator::next() {

		return ++lastId_;

	}
	
	idGenerator::idGenerator(const idGenerator & o)		// copy ctor

		: lastId_(o.lastId_) {}

	idGenerator & idGenerator::operator =(const idGenerator & o) {
	
		if (&o != this) {
		
			lastId_ = o.lastId_;
		
		}

		return *this;
	
	}

}