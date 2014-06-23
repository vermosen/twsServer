#ifndef tws_function_static_data_request
#define tws_function_static_data_request

#include <boost/lexical_cast.hpp>

// thOth components
#include <thOth/time/DateTime.hpp>
#include <thOth/utilities/csvBuilder.hpp>

#include "utilities/type/all.hpp"
#include "utilities/settings.hpp"
#include "utilities/define.hpp"
#include "staticDataRequestClient/staticDataRequestClient.hpp"

// mySQL
#include <my_global.h>
#include <mysql.h>

// this procedure is meant to request 
// missing static data about a given contract
void staticDataRequest();

#endif