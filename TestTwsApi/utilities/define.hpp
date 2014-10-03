/*
 *
 * custom TwsApi
 * Copyright (C) 2014 Jean-Mathieu Vermosen
 *
 */
#ifndef tws_define_hpp
#define tws_define_hpp

#include <boost/date_time/posix_time/posix_time.hpp>

#include "utilities/settings/settings.hpp"

#define TWS_LOG_V(X, Y) \
	if (IB::settings::instance().verbosity() > Y) \
	IB::settings::instance().log().push_back( \
		boost::posix_time::to_iso_string( \
			boost::posix_time::microsec_clock::local_time()) \
				.append(", ") \
				.append(X));


const unsigned MAX_ATTEMPT = 2;				// maximum number of connection attemps
const unsigned SLEEP_TIME_H  = 400;			// sleep time in ms between two historical request
const unsigned SLEEP_TIME    = 100;			// sleep time after an unsuccessful request

#endif