#ifndef tws_define_hpp
#define tws_define_hpp

// macros
#define TWS_LOG(X) \
	IB::settings::instance().log()->push_back( \
		boost::posix_time::to_iso_string( \
			boost::posix_time::microsec_clock::local_time()) \
				.append(", ") \
				.append(X));

#endif