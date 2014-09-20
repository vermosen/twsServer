#ifndef tws_function_bulk_import
#define tws_function_bulk_import

#include <thread>

#include <boost/lexical_cast.hpp>
#include <boost/timer.hpp>

// thOth components
#include <thOth/time/DateTime.hpp>

// twsapi
#include "request/historicalRequestClient/historicalRequestClient.hpp"
#include "recordset/tableContractRecordset/tableContractRecordset.hpp"
#include "recordset/functions/singleHistoricalBarInsert/singleHistoricalBarInsert.hpp"

// this procedure is meant to request 
// historical data for all the contracts in the database
// opt1: start date string
// opt2: end date string
void bulkImport(const thOth::dateTime &,
				const thOth::dateTime &,
				bool deletionPolicy = false);

#endif