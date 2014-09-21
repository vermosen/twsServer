#include "recordset/functions/singleHistoricalBarSelect/singleHistoricalBarSelect.hpp"

boost::shared_ptr<std::vector<thOth::bar> > singleHistoricalBarSelect(
	const IB::dataBase::contractRecord & contract_,
	const thOth::dateTime & startDate_,
	const thOth::dateTime & endDate_) {

	boost::shared_ptr<std::vector<thOth::bar> > returnVec (new std::vector < thOth::bar >());

	// step 1: initialize connection
	IB::dataBase::tableHistoricalBarRecordset rs(
		IB::settings::instance().connection());

	// step 2: building the query
	std::string query;
	query.append("SELECT * FROM table_historical_bar WHERE (contract_ID = ");
	INSERT_SQL_NUM(query, contract_.first)
		query.append(" AND bar_start >= ");
	INSERT_SQL_DATE(query, startDate_)
		query.append(" AND bar_end <= ");
	INSERT_SQL_DATE(query, endDate_)
		query.append(")");

	rs.selectQ(query);											// run the query
	
	for (IB::dataBase::tableHistoricalBarRecordset::const_iterator It
		= rs.cbegin(); It != rs.cend(); It++)
		returnVec->push_back(It->second.bar());					// insert the data in the vector

	std::sort(returnVec->begin(), returnVec->end());				// sort the vector

	return returnVec;

};