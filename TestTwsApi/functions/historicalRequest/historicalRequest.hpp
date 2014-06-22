#ifndef tws_function_historical_request
#define tws_function_historical_request

#include <boost/lexical_cast.hpp>

// thOth components
#include <thOth/time/DateTime.hpp>
#include <thOth/utilities/csvBuilder.hpp>

#include "utilities/type/all.hpp"
#include "utilities/settings.hpp"
#include "historicalRequestClient/historicalRequestClient.hpp"
#include "utilities/define.hpp"

// mySQL
#include <my_global.h>
#include <mysql.h>

// max connection attempts
const unsigned MAX_ATTEMPT  = 2;
const unsigned SLEEP_TIME   = 5;

void historicalRequest();

#endif