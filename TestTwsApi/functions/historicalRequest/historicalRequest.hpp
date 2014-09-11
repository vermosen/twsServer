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



// this procedure is meant to request 
// missing historical data about a given contract
// opt1: contract name
// opt2: date string
void historicalRequest(const std::string & opt1 = std::string(""),
					   const std::string & opt2 = std::string(""),
					   const std::string & opt3 = std::string(""));

#endif