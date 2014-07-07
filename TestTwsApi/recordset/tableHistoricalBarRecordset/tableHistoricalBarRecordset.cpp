#include "recordset/tableHistoricalBarRecordset/tableHistoricalBarRecordset.hpp"

namespace IB {

	namespace dataBase {
	
		tableHistoricalBarRecordset::tableHistoricalBarRecordset(MYSQL * connection) 
			: recordset(connection) {};

		tableHistoricalBarRecordset::tableHistoricalBarRecordset(const tableHistoricalBarRecordset & o) {
		
			records_ = o.records_;
		
		};

		tableHistoricalBarRecordset::~tableHistoricalBarRecordset() {};

		tableHistoricalBarRecordset & tableHistoricalBarRecordset::operator=(const tableHistoricalBarRecordset & o) {
		
			recordset::operator=(o);						// call the parent class =

			if (this != &o) {
			
				// copy some stuff
				records_ = o.records_;

			}

			return *this;
		
		}

		bool tableHistoricalBarRecordset::open() {
		
			return true;
		
		};

		void tableHistoricalBarRecordset::close() {
		

		
		};

		bool tableHistoricalBarRecordset::select(const std::string & selectStr) {
		
			mysql_query(												// query to run
				connection_,
				selectStr.c_str());

			reception_ = mysql_store_result(connection_);
		
			if (!reception_)											// sql statement failed
				return false;

			if (reception_->row_count == 0)								// no record
				return false;

			// local copy in records_
			//records_.insert();
			return true;

		};

		bool tableHistoricalBarRecordset::insert(const barRecord & rec) {

			std::string fieldStr, valueStr;								// the two fields to build together

			fieldStr.append("CONTRACT_ID,");							// contract id

			valueStr
				.append(boost::lexical_cast<std::string>(rec.id_))
				.append(",");

			fieldStr.append("BAR_START,");								// barStart

			valueStr
				.append("'")
				.append(convertDateTime(rec.bar_.barStart()))
				.append("',");
			
			fieldStr.append("BAR_END,");								// barEnd

			valueStr
				.append("'")
				.append(convertDateTime(rec.bar_.barEnd()))
				.append("',");
			
			fieldStr.append("OPEN,");									// open

			valueStr
				.append(boost::lexical_cast<std::string>(rec.bar_.open()))
				.append(",");
			
			fieldStr.append("CLOSE,");									// close

			valueStr
				.append(boost::lexical_cast<std::string>(rec.bar_.close()))
				.append(",");
			
			fieldStr.append("HIGH,");									// close

			valueStr
				.append(boost::lexical_cast<std::string>(rec.bar_.high()))
				.append(",");

			fieldStr.append("LOW,");									// close

			valueStr
				.append(boost::lexical_cast<std::string>(rec.bar_.low()))
				.append(",");

			fieldStr.append("VOLUME,");									// close

			valueStr
				.append(boost::lexical_cast<std::string>(rec.bar_.volume()))
				.append(",");

			fieldStr.append("EXCHANGE");									// close

			valueStr
				.append("'")
				.append(rec.exchange_)
				.append("'");

			std::string insertStatement("INSERT INTO table_contract (");
			insertStatement
				.append(fieldStr)
				.append(") VALUES (")
				.append(valueStr)
				.append(")");

			if (mysql_query(connection_, insertStatement.c_str()) != 0)		// throw on an error
				throw std::exception(mysql_error(connection_));

			return true;													// return true otherwise

		}
	}
}