#ifndef tws_define_hpp
#define tws_define_hpp

// build logging string
#define TWS_LOG(X) \
	IB::settings::instance().log()->push_back( \
		boost::posix_time::to_iso_string( \
			boost::posix_time::microsec_clock::local_time()) \
				.append(", ") \
				.append(X));

// maximum number of connection attemps
const unsigned MAX_ATTEMPT = 2;

// sleep time between two connections
const unsigned SLEEP_TIME = 5;

#endif