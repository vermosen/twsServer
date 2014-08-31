#ifndef tws_bulk_import
#define tws_bulk_import

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
// historical data for all the contracts in the database
// opt1: date string
void bulkImport(const std::string & opt1 = std::string(""));

#endif