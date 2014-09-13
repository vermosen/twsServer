#ifndef tws_function_static_data_request
#define tws_function_static_data_request

#include <boost/lexical_cast.hpp>
#include <boost/timer.hpp>
#include <boost/thread.hpp>

// thOth components
#include <thOth/time/DateTime.hpp>

#include "utilities/type/all.hpp"
#include "utilities/settings/settings.hpp"
#include "utilities/define.hpp"
#include "request/staticDataRequestClient/staticDataRequestClient.hpp"
#include "recordset/tableContractRecordset/tableContractRecordset.hpp"

// this procedure is meant to request 
// missing static data about a given contract
// opt1
void staticDataRequest(bool deletionPolicy, 
					   const std::string & = std::string(""));

#endif