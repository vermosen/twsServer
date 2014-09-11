#ifndef tws_function_history_file_creation
#define tws_function_history_file_creation

#include <boost/lexical_cast.hpp>
#include <boost/timer.hpp>

// thOth components
#include <thOth/time/DateTime.hpp>
#include <thOth/utilities/csvBuilder.hpp>

// twsapi
#include "utilities/type/all.hpp"
#include "utilities/settings/settings.hpp"
#include "utilities/define.hpp"
#include "recordset/tableContractRecordset/tableContractRecordset.hpp"
#include "recordset/tableHistoricalBarRecordset/tableHistoricalBarRecordset.hpp"

// this procedure implements a simple strategy
// opt1: name of the contract to study
// TODO: enable opt2 and opt3 for time boundaries
void historyFileCreation(const std::string & opt1 = std::string(""),
						 const std::string & opt2 = std::string(""),
						 const std::string & opt3 = std::string(""));

#endif