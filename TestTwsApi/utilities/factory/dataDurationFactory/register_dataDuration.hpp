#ifndef register_bar_size_hpp
#define register_bar_size_hpp

#include <utilities/factory/barSizeFactory/barSizeFactory.hpp>

namespace IB {

	namespace utilities {

		void registerBarSize() {

			barSizeFactory factory;

			createDataDuration.registerType("1 sec", new IB::barSize(IB::oneSecond));
			createDataDuration.registerType("5 secs", new IB::barSize(IB::fiveSeconds));
			createDataDuration.registerType("15 secs", new IB::barSize(IB::fifteenSeconds));
			createDataDuration.registerType("30 secs", new IB::barSize(IB::thirtySeconds));
			createDataDuration.registerType("1 min", new IB::barSize(IB::oneMinute));
			createDataDuration.registerType("2 mins", new IB::barSize(IB::twoMinutes));
			createBarSize.registerType("3 mins"      , new IB::barSize(IB::threeMinutes  ));
			createBarSize.registerType("5 mins"      , new IB::barSize(IB::fiveMinutes   ));
			createBarSize.registerType("15 mins"     , new IB::barSize(IB::fifteenMinutes));
			createBarSize.registerType("30 mins"     , new IB::barSize(IB::thirtyMinutes ));
			createBarSize.registerType("1 hour"      , new IB::barSize(IB::oneHour       ));
			createBarSize.registerType("1 day"       , new IB::barSize(IB::oneDay        ));
			createBarSize.registerType("unknown size", new IB::barSize(IB::unknownSize   ));

		};

	}

}

#endif
