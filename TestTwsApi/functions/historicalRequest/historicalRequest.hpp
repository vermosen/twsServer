#ifndef tws_function_historical_request
#define tws_function_historical_request

#include <boost/lexical_cast.hpp>
#include <boost/timer.hpp>
#include <boost/thread.hpp>

// thOth components
#include <thOth/time/DateTime.hpp>

// twsapi
#include "utilities/type/all.hpp"
#include "utilities/define.hpp"
#include "utilities/settings/settings.hpp"
#include "request/historicalRequestClient/historicalRequestClient.hpp"
#include "recordset/tableContractRecordset/tableContractRecordset.hpp"
#include "recordset/tableHistoricalBarRecordset/tableHistoricalBarRecordset.hpp"

// this procedure is meant to request 
// missing historical data about a given contract
void historicalRequest(const std::string &, const std::string &);

#endif