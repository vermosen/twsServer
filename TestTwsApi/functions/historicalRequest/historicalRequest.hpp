#ifndef tws_function_historical_request
#define tws_function_historical_request

#include <functional>
#include <thread>
#include <chrono>

// boost components
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
#include "recordset/functions/singleHistoricalBarInsert/singleHistoricalBarInsert.hpp"

// this procedure is meant to request 
// missing historical data about a given contract
void historicalRequest(IB::dataBase::contractRecord contract_,
					   const thOth::dateTime & startDate,
					   const thOth::dateTime & endDate,
					   bool deletionPolicy);
#endif