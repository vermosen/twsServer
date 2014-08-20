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

}