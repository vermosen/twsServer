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

			MYSQL_ROW row;

			while (row = mysql_fetch_row(reception_)) {					// loop over the results
				// !! check how it iterates !!
				// contract details might be partial 
				// depending of the original request
				thOth::bar currentBar;								// the current contract

				for (long i = 0; i < reception_->field_count; i++) {

					if (std::string(reception_->fields[i].name)
						== "CONTRACT_ID" && row[i] != NULL)
						contract.summary.conId = boost::lexical_cast<long>(row[i]);

					else if (std::string(reception_->fields[i].name)
						== "CONTRACT_IBID" && row[i] != NULL)
						contract.summary.conId = boost::lexical_cast<long>(row[i]);

					else if (std::string(reception_->fields[i].name)
						== "CONTRACT_SYMBOL" && row[i] != NULL)
						contract.summary.symbol = std::string(row[i]);

					else if (std::string(reception_->fields[i].name)
						== "CONTRACT_SECTYPE" && row[i] != NULL)
						contract.summary.secType = std::string(row[i]);

					else if (std::string(reception_->fields[i].name)
						== "CONTRACT_EXPIRY" && row[i] != NULL)
						contract.summary.expiry = std::string(row[i]);

					else if (std::string(reception_->fields[i].name)
						== "CONTRACT_STRIKE" && row[i] != NULL)
						contract.summary.strike = boost::lexical_cast<double>(row[i]);

					else if (std::string(reception_->fields[i].name)
						== "CONTRACT_RIGHT" && row[i] != NULL) {

						// temporary variable
						unsigned int j = boost::lexical_cast<unsigned int>(row[i]);

						(j == 1 ? contract.summary.right = "P" :
							(j == 2 ? contract.summary.right = "C" :
							contract.summary.right = ""));

					}

					else if (std::string(reception_->fields[i].name)
						== "CONTRACT_MULTIPLIER" && row[i] != NULL)
						contract.summary.multiplier = std::string(row[i]);

					else if (std::string(reception_->fields[i].name)
						== "CONTRACT_PRIMARY_EXCHANGE" && row[i] != NULL)
						contract.summary.primaryExchange = std::string(row[i]);

					else if (std::string(reception_->fields[i].name)
						== "CONTRACT_CURRENCY" && row[i] != NULL)
						contract.summary.currency = std::string(row[i]);

					else if (std::string(reception_->fields[i].name)
						== "CONTRACT_TRADING_CLASS" && row[i] != NULL)
						contract.summary.tradingClass = std::string(row[i]);

					// might be the cusip plus another identifier
					// strategy: first insert cusip only in bond.cusip
					// then complete if this is the only identifier
					else if (std::string(reception_->fields[i].name)
						== "CONTRACT_CUSIP" && row[i] != NULL)
						contract.cusip = std::string(row[i]);

					else if (std::string(reception_->fields[i].name)
						== "CONTRACT_ISIN" && row[i] != NULL) {

						contract.summary.secId = std::string(row[i]);
						contract.summary.secIdType = "ISIN";

					}

					else if (std::string(reception_->fields[i].name)
						== "CONTRACT_SEDOL" && row[i] != NULL) {

						contract.summary.secId = std::string(row[i]);
						contract.summary.secIdType = "SEDOL";

					}

					else if (std::string(reception_->fields[i].name)
						== "CONTRACT_RIC" && row[i] != NULL) {

						contract.summary.secId = std::string(row[i]);
						contract.summary.secIdType = "RIC";

					}

					// other fields
					else if (std::string(reception_->fields[i].name)
						== "CONTRACT_DETAILS_MARKET_NAME" && row[i] != NULL)
						contract.marketName = std::string(row[i]);

					else if (std::string(reception_->fields[i].name)
						== "CONTRACT_DETAILS_MINTICK" && row[i] != NULL)
						contract.minTick = boost::lexical_cast<double>(row[i]);

					else if (std::string(reception_->fields[i].name)
						== "CONTRACT_DETAILS_ORDERTYPES" && row[i] != NULL)
						contract.orderTypes = std::string(row[i]);

					else if (std::string(reception_->fields[i].name)
						== "CONTRACT_DETAILS_VALID_EXCHANGES" && row[i] != NULL)
						contract.validExchanges = std::string(row[i]);

					else if (std::string(reception_->fields[i].name)
						== "CONTRACT_DETAILS_PRICE_MAGNIFIER" && row[i] != NULL)
						contract.priceMagnifier = boost::lexical_cast<long>(row[i]);

					else if (std::string(reception_->fields[i].name)
						== "CONTRACT_DETAILS_UNDERLYING_CONTRACT_ID" && row[i] != NULL)
						contract.underConId = boost::lexical_cast<long>(row[i]);

					else if (std::string(reception_->fields[i].name)
						== "CONTRACT_DETAILS_LONG_NAME" && row[i] != NULL)
						contract.longName = std::string(row[i]);

					else if (std::string(reception_->fields[i].name)
						== "CONTRACT_DETAILS_MONTH" && row[i] != NULL)
						contract.contractMonth = std::string(row[i]);

					else if (std::string(reception_->fields[i].name)
						== "CONTRACT_DETAILS_INDUSTRY" && row[i] != NULL)
						contract.industry = std::string(row[i]);

					else if (std::string(reception_->fields[i].name)
						== "CONTRACT_DETAILS_CATEGORY" && row[i] != NULL)
						contract.category = std::string(row[i]);

					else if (std::string(reception_->fields[i].name)
						== "CONTRACT_DETAILS_SUBCATEGORY" && row[i] != NULL)
						contract.subcategory = std::string(row[i]);

					else if (std::string(reception_->fields[i].name)
						== "CONTRACT_DETAILS_TIMEZONE_ID" && row[i] != NULL)
						contract.timeZoneId = std::string(row[i]);

					else if (std::string(reception_->fields[i].name)
						== "CONTRACT_DETAILS_TRADINGHOURS" && row[i] != NULL)
						contract.tradingHours = std::string(row[i]);

					else if (std::string(reception_->fields[i].name)
						== "CONTRACT_DETAILS_LIQUIDHOURS" && row[i] != NULL)
						contract.liquidHours = std::string(row[i]);

					else if (std::string(reception_->fields[i].name)
						== "CONTRACT_DETAILS_EVRULE" && row[i] != NULL)
						contract.evRule = std::string(row[i]);

					else if (std::string(reception_->fields[i].name)
						== "CONTRACT_DETAILS_EVRULE_MULTIPLIER" && row[i] != NULL)
						contract.evMultiplier = boost::lexical_cast<long>(row[i]);

					else if (std::string(reception_->fields[i].name)
						== "CONTRACT_DETAILS_BOND_RATINGS" && row[i] != NULL)
						contract.ratings = std::string(row[i]);

					else if (std::string(reception_->fields[i].name)
						== "CONTRACT_DETAILS_BOND_DESCRIPTION" && row[i] != NULL)
						contract.descAppend = std::string(row[i]);

					else if (std::string(reception_->fields[i].name)
						== "CONTRACT_DETAILS_BOND_TYPE" && row[i] != NULL)
						contract.bondType = std::string(row[i]);

					else if (std::string(reception_->fields[i].name)
						== "CONTRACT_DETAILS_BOND_COUPONTYPE" && row[i] != NULL)
						contract.couponType = std::string(row[i]);

					else if (std::string(reception_->fields[i].name)
						== "CONTRACT_DETAILS_BOND_CALLABLE" && row[i] != NULL)
						(std::string(row[i]) == "1" ?
						contract.callable = true :
						contract.callable = false);

					else if (std::string(reception_->fields[i].name)
						== "CONTRACT_DETAILS_BOND_PUTABLE" && row[i] != NULL)
						(std::string(row[i]) == "1" ?
						contract.putable = true :
						contract.putable = false);

					else if (std::string(reception_->fields[i].name)
						== "CONTRACT_DETAILS_BOND_COUPON" && row[i] != NULL)
						contract.couponType = std::string(row[i]);

					else if (std::string(reception_->fields[i].name)
						== "CONTRACT_DETAILS_BOND_CONVERTIBLE" && row[i] != NULL)
						(std::string(row[i]) == "1" ?
						contract.convertible = true :
						contract.convertible = false);

					else if (std::string(reception_->fields[i].name)
						== "CONTRACT_DETAILS_BOND_MATURITY" && row[i] != NULL)
						contract.maturity = std::string(row[i]);

					else if (std::string(reception_->fields[i].name)
						== "CONTRACT_DETAILS_BOND_ISSUEDATE" && row[i] != NULL)
						contract.issueDate = std::string(row[i]);

					else if (std::string(reception_->fields[i].name)
						== "CONTRACT_DETAILS_BOND_NEXTOPTIONDATE" && row[i] != NULL)
						contract.nextOptionDate = std::string(row[i]);

					else if (std::string(reception_->fields[i].name)
						== "CONTRACT_DETAILS_BOND_NEXTOPTIONTYPE" && row[i] != NULL)
						contract.nextOptionType = std::string(row[i]);

					else if (std::string(reception_->fields[i].name)
						== "CONTRACT_DETAILS_BOND_NEXTOPTIONTYPE" && row[i] != NULL)
						contract.nextOptionType = std::string(row[i]);

					else if (std::string(reception_->fields[i].name)
						== "CONTRACT_DETAILS_BOND_NEXTOPTIONPARTIAL" && row[i] != NULL)
						(std::string(row[i]) == "1" ?
						contract.nextOptionPartial = true :
						contract.nextOptionPartial = false);

					else if (std::string(reception_->fields[i].name)
						== "CONTRACT_DETAILS_BOND_NOTE" && row[i] != NULL)
						contract.notes = std::string(row[i]);

					else
						std::exception("unknown field provided");

				}

				// in case the cusip has been copied
				if (contract.summary.secType == "" && contract.cusip != "") {

					contract.summary.secId = contract.cusip;
					contract.summary.secIdType = "CUSIP";

				}

				// finally insert the contract details in the map
				records_.insert(
					std::pair<recordId, thOth::bar>(
						boost::lexical_cast<recordId>(row[0]), bar));

			}

			return true;

		};

		bool tableHistoricalBarRecordset::insert(const barRecord & rec) {

			std::string fieldStr, valueStr;								// the two fields to build together

			fieldStr.append("CONTRACT_ID,");							// contract id

			INSERT_SQL_STR(
				valueStr,
				boost::lexical_cast<std::string>(rec.id_))

			fieldStr.append("BAR_START,");								// barStart

			INSERT_SQL_STR(
				valueStr, 
				convertDateTime(rec.bar_.barStart()))
			
			fieldStr.append("BAR_END,");								// barEnd

			INSERT_SQL_STR(
				valueStr, 
				convertDateTime(rec.bar_.barEnd()))
			
			fieldStr.append("OPEN,");									// open

			INSERT_SQL_STR(
				valueStr, 
				boost::lexical_cast<std::string>(
					rec.bar_.open()))
			
			fieldStr.append("CLOSE,");									// close

			INSERT_SQL_STR(
				valueStr,
				boost::lexical_cast<std::string>(
					rec.bar_.close()))
			
			fieldStr.append("HIGH,");									// close

			INSERT_SQL_STR(
				valueStr,
				boost::lexical_cast<std::string>(
					rec.bar_.high()))

			fieldStr.append("LOW,");									// close

			INSERT_SQL_STR(
				valueStr,
				boost::lexical_cast<std::string>(
					rec.bar_.low()))

			fieldStr.append("VOLUME,");									// close

			INSERT_SQL_STR(
				valueStr,
				boost::lexical_cast<std::string>(
					rec.bar_.volume()))

			fieldStr.append("EXCHANGE");								// close

			INSERT_SQL_STR(
				valueStr,
				boost::lexical_cast<std::string>(
				rec.exchange_))

			valueStr.pop_back();										// remove the last colon
			
			std::string insertStatement("INSERT INTO table_contract (");
			
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