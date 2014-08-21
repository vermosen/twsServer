#ifndef tws_function_multi_threading
#define tws_function_multi_threading

#include <thread>

#include <boost/lexical_cast.hpp>
#include <boost/timer.hpp>

// thOth components
#include <thOth/time/DateTime.hpp>

// twsapi
#include "utilities/type/all.hpp"
#include "utilities/settings.hpp"
#include "utilities/define.hpp"
#include "EWrapper/historicalRequestClient/historicalRequestClient.hpp"
#include "recordset/tableContractRecordset/tableContractRecordset.hpp"
#include "recordset/tableHistoricalBarRecordset/tableHistoricalBarRecordset.hpp"

// this procedure implements a simple strategy
void simpleStrategy();

#endif