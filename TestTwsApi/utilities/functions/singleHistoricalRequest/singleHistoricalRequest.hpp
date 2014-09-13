#ifndef tws_single_historical_request
#define tws_single_historical_request

#include <functional>
#include <thread>

// boost components
#include <boost/lexical_cast.hpp>
#include <boost/timer.hpp>

// thOth components
#include <thOth/time/DateTime.hpp>

// twsapi
#include "utilities/type/all.hpp"
#include "utilities/define.hpp"
#include "request/historicalRequestClient/historicalRequestClient.hpp"
#include "recordset/tableContractRecordset/tableContractRecordset.hpp"
#include "recordset/tableHistoricalBarRecordset/tableHistoricalBarRecordset.hpp"

// single date insert function
void singleHistoricalRequest(IB::dataBase::recordId id_,
							 IB::dataBase::tableHistoricalBarRecordset & barRs_,
							 const IB::ContractDetails & contract_,
							 thOth::dateTime dt_,
							 bool policy_);

#endif