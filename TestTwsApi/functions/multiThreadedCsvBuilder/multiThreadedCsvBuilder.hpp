#ifndef tws_function_multi_threaded_csv_builder
#define tws_function_multi_threaded_csv_builder

#include <thread>
#include <random>

#include <boost/lexical_cast.hpp>
#include <boost/timer.hpp>

// twsapi
#include "utilities/type/all.hpp"
#include "utilities/settings.hpp"
#include "utilities/define.hpp"

// this procedure is meant to test multi-threading
void multiThreadedCsvBuilder();

#endif