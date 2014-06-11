#ifndef bar_size_hpp
#define bar_size_hpp

#include <IBString.h>

namespace IB {
	
	enum barSize : unsigned int{

		oneSecond      = 0,
		fiveSeconds    = 1,
		fifteenSeconds = 2,
		thirtySeconds  = 3,
		oneMinute      = 4,
		twoMinutes     = 5,
		threeMinutes   = 6,
		fiveMinutes    = 7,
		fifteenMinutes = 8,
		thirtyMinutes  = 9,
		oneHour        = 10,
		oneDay         = 11,
		unknownSize    = 12

	};

	// conversion into the corresponding IB strings
	inline const IBString ToString(barSize v) {

		switch (v) {

			case oneSecond     : return "1 sec"      ;
			case fiveSeconds   : return "5 secs"     ;
			case fifteenSeconds: return "15 secs"    ;
			case thirtySeconds : return "30 secs"    ;
			case oneMinute     : return "1 min"      ;
			case twoMinutes    : return "2 mins"     ;
			case threeMinutes  : return "3 mins"     ;
			case fiveMinutes   : return "5 mins"     ;
			case fifteenMinutes: return "15 mins"    ;
			case thirtyMinutes : return "30 mins"    ;
			case oneHour       : return "1 hour"     ;
			case oneDay        : return "1 day"      ;
			case unknownSize   : return "unkown size";

			default            : return "unkown size";
		
		}
	}
}



#endif