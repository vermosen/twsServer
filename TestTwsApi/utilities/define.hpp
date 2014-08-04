/*
 *
 * custom TwsApi
 * Copyright (C) 2014 Jean-Mathieu Vermosen
 *
 */
#ifndef tws_define_hpp
#define tws_define_hpp

#include <thOth/time/DateTime.hpp>

#include "utilities/settings.hpp"

// build logging string macro
#define TWS_LOG(X) \
	IB::settings::instance().log().push_back( \
		boost::posix_time::to_iso_string( \
			boost::posix_time::microsec_clock::local_time()) \
				.append(", ") \
				.append(X));


const unsigned MAX_ATTEMPT = 1;				// maximum number of connection attemps
const unsigned SLEEP_TIME  = 3;				// sleep time between two connections

// sleep macro
#ifdef _WIN32
//#include <windows.h>						// link bug
#define sleep( seconds) Sleep( seconds * 1000);
#else
#include <unistd.h>
#endif

#endif