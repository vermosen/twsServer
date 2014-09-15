#ifndef tws_simple_strategy
#define tws_simple_strategy

#include <thread>
#include <memory>

#include <boost/lexical_cast.hpp>
#include <boost/timer.hpp>

// thOth components
#include <thOth/time/DateTime.hpp>

// twsapi
#include "utilities/type/all.hpp"
#include "utilities/define.hpp"
#include "utilities/settings/settings.hpp"
#include "request/historicalRequestClient/historicalRequestClient.hpp"
#include "recordset/tableContractRecordset/tableContractRecordset.hpp"
#include "recordset/tableHistoricalBarRecordset/tableHistoricalBarRecordset.hpp"

// this function is meant to test a simple signal strategy over a time series
void simpleStrategy(const std::pair<IB::dataBase::recordId,IB::ContractDetails> & contract_,
				    const thOth::dateTime & startDate_,
				    const thOth::dateTime & endDate_,
					bool deletionPolicy_ = false);

#endif