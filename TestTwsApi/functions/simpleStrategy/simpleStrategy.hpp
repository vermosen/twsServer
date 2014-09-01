#ifndef tws_function_multi_threading
#define tws_function_multi_threading

#include <thread>

#include <boost/lexical_cast.hpp>
#include <boost/timer.hpp>

// thOth components
#include <thOth/time/DateTime.hpp>

// twsapi
#include "utilities/type/all.hpp"
#include "utilities/settings/settings.hpp"
#include "utilities/define.hpp"
#include "recordset/tableContractRecordset/tableContractRecordset.hpp"
#include "recordset/tableHistoricalBarRecordset/tableHistoricalBarRecordset.hpp"

// this procedure implements a simple strategy
// opt1: name of the contract to study
void simpleStrategy(const std::string & opt = std::string(""));

#endif