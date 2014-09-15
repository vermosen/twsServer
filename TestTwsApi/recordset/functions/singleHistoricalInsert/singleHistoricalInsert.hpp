#ifndef single_historical_insert_hpp
#define single_historical_insert_hpp

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
void singleHistoricalInsert(
	const std::pair<IB::dataBase::recordId, IB::ContractDetails> & contract_,
	IB::dataBase::tableHistoricalBarRecordset & barRs_,
	thOth::dateTime dt_,
	bool policy_);

#endif