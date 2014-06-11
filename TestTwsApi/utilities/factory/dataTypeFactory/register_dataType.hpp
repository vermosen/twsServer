#ifndef register_data_type_hpp
#define register_data_type_hpp

#include <utilities/factory/dataTypeFactory/dataTypeFactory.hpp>

namespace IB {

	namespace utilities {

		void registerBarSize() {

			dataTypeFactory createBarSize;

			createBarSize.registerType("TRADES", new IB::dataType(IB::trades));
			createBarSize.registerType("MIDPOINT", new IB::dataType(IB::midPoint));
			createBarSize.registerType("BID", new IB::dataType(IB::bid));
			createBarSize.registerType("ASK", new IB::dataType(IB::ask));
			createBarSize.registerType("BID_ASK", new IB::dataType(IB::bidAsk));
			createBarSize.registerType("HISTORICAL_VOLATILITY", new IB::dataType(IB::historicalVolatility));
			createBarSize.registerType("OPTION_IMPLIED_VOLATILITY", new IB::dataType(IB::optionImpliedVolatility));
			createBarSize.registerType("unkown size", new IB::dataType(IB::unknownType));

		};

	}

}

#endif
