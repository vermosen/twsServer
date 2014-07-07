#ifndef tws_function_historical_request
#define tws_function_historical_request

#include <boost/lexical_cast.hpp>

// thOth components
#include <thOth/time/DateTime.hpp>
#include <thOth/utilities/csvBuilder.hpp>

// twsapi
#include "utilities/type/all.hpp"
#include "utilities/settings.hpp"
#include "utilities/define.hpp"
#include "historicalRequestClient/historicalRequestClient.hpp"
#include "recordset/tableContractRecordset/tableContractRecordset.hpp"

// mySQL
#include <mysql.h>

// this procedure is meant to request 
// missing historical data about a given contract
void historicalRequest();

#endif