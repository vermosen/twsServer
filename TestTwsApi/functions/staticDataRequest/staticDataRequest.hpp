#ifndef tws_function_static_data_request
#define tws_function_static_data_request

#include <boost/lexical_cast.hpp>
#include <boost/timer.hpp>

// thOth components
#include <thOth/time/DateTime.hpp>

#include "utilities/type/all.hpp"
#include "utilities/settings.hpp"
#include "utilities/define.hpp"
#include "EWrapper/staticDataRequestClient/staticDataRequestClient.hpp"
#include "recordset/tableContractRecordset/tableContractRecordset.hpp"

// this procedure is meant to request 
// missing static data about a given contract
void staticDataRequest();

#endif