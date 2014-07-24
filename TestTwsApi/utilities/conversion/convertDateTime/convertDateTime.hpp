#ifndef convert_date_time
#define convert_date_time

#include <thOth/time/DateTime.hpp>
#include <IBString.h>

thOth::dateTime convertDateTime(const IB::IBString &   );				// parse a date string into some dateTime
IB::IBString    convertDateTime(const thOth::dateTime &);

#endif