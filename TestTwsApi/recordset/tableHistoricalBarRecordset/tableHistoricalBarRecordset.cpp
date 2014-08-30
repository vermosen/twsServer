#include "recordset/tableHistoricalBarRecordset/tableHistoricalBarRecordset.hpp"

namespace IB {

	namespace dataBase {
	
		barRecord::barRecord(
			recordId id,
			const thOth::bar & bar,
			const std::string & exchange) 
			: id_(id), bar_(bar), exchange_(exchange) {};
		
		barRecord::barRecord(const barRecord & rec)
			: id_(rec.id_), bar_(rec.bar_), exchange_(rec.exchange_) {};

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

		bool tableHistoricalBarRecordset::select(const std::string & selectStr) {
		
			mysql_query(												// query to run
				connection_,
				selectStr.c_str());

			reception_ = mysql_store_result(connection_);
		
			if (!reception_)											// sql statement failed
				throw selectQueryExceptionSelectFailed();

			if (reception_->row_count == 0)								// no record
				throw selectQueryExceptionNoSelection();

			MYSQL_ROW row;

			recordId barId = 1;											// in case the id is not part of the 
																		// request we generate a key

			while (row = mysql_fetch_row(reception_)) {					// loop over the results
				
				recordId instrumentId;									// the id of the record to insert
				thOth::dateTime startDate; thOth::dateTime endDate;		// mandatory bar components
				thOth::real open; thOth::real close; thOth::real low;
				thOth::real high; thOth::real volume;
				std::string exchange;

				for (unsigned long i = 0;								// loop over the fields
					i < reception_->field_count; i++) {	

					if (std::string(reception_->fields[i].name)
						== "bar_id" && row[i] != NULL)
						barId = boost::lexical_cast<recordId>(row[i]);

					else if (std::string(reception_->fields[i].name)
						== "contract_id" && row[i] != NULL)
						instrumentId = boost::lexical_cast<recordId>(row[i]);

					else if (std::string(reception_->fields[i].name)
						== "bar_start" && row[i] != NULL)
						startDate = convertDateTime(IB::IBString(row[i]));

					else if (std::string(reception_->fields[i].name)
						== "bar_end" && row[i] != NULL)
						endDate   = convertDateTime(IB::IBString(row[i]));

					else if (std::string(reception_->fields[i].name)
						== "open" && row[i] != NULL)
						open = boost::lexical_cast<thOth::real>(row[i]);

					else if (std::string(reception_->fields[i].name)
						== "close" && row[i] != NULL)
						open = boost::lexical_cast<thOth::real>(row[i]);

					else if (std::string(reception_->fields[i].name)
						== "close" && row[i] != NULL)
						close = boost::lexical_cast<thOth::real>(row[i]);

					else if (std::string(reception_->fields[i].name)
						== "high" && row[i] != NULL)
						high = boost::lexical_cast<thOth::real>(row[i]);

					else if (std::string(reception_->fields[i].name)
						== "low" && row[i] != NULL)
						low = boost::lexical_cast<thOth::real>(row[i]);
					
					else if (std::string(reception_->fields[i].name)
						== "volume" && row[i] != NULL)
						volume = boost::lexical_cast<thOth::real>(row[i]);

					else

						throw selectQueryExceptionUnknownField();
						//std::exception("unknown field provided");

				}

				records_.insert(									// finally insert the contract details in the map
					std::pair<recordId, barRecord>(
						barId,
						barRecord(
							instrumentId,
							thOth::bar(
								startDate, endDate,
								open, close, high, 
								low, volume),
							exchange)));

				barId++;

			}

			return true;

		};

		bool tableHistoricalBarRecordset::insert(const barRecord & rec) {

			std::string fieldStr, valueStr;								// the two fields to build together

			fieldStr.append("CONTRACT_ID,");							// contract id

			INSERT_SQL_STR(
				valueStr,
				boost::lexical_cast<std::string>(rec.instrumentIdentifier()))

			fieldStr.append("BAR_START,");								// barStart

			INSERT_SQL_DATE(
				valueStr, 
				rec.bar().barStart())

			fieldStr.append("BAR_END,");								// barEnd

			INSERT_SQL_DATE(
				valueStr, 
				rec.bar().barEnd())
			
			fieldStr.append("OPEN,");									// open

			INSERT_SQL_NUM(
				valueStr, 
				rec.bar().open())
			
			fieldStr.append("CLOSE,");									// close

			INSERT_SQL_NUM(
				valueStr,
				rec.bar().close())
			
			fieldStr.append("HIGH,");									// close

			INSERT_SQL_NUM(
				valueStr,
				rec.bar().high())

			fieldStr.append("LOW,");									// close

			INSERT_SQL_NUM(
				valueStr,
				rec.bar().low())

			fieldStr.append("VOLUME,");									// close

			INSERT_SQL_NUM(
				valueStr,
				rec.bar().volume())

			fieldStr.append("EXCHANGE");								// close

			INSERT_SQL_STR(
				valueStr,
				boost::lexical_cast<std::string>(
				rec.exchange()))

			valueStr.pop_back();										// remove the last colon
			
			std::string insertStatement("INSERT INTO table_historical_bar (");
			
			insertStatement
				.append(fieldStr)
				.append(") VALUES (")
				.append(valueStr)
				.append(")");

			if (mysql_query(connection_, insertStatement.c_str()) != 0)	// throw on an error
				throw std::exception(mysql_error(connection_));

			return true;												// return true otherwise

		}
	}
}