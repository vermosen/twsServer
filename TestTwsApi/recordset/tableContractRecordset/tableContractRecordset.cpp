#include "recordset/tableContractRecordset/tableContractRecordset.hpp"

namespace IB {

	namespace dataBase {
	
		tableContractRecordset::tableContractRecordset(MYSQL * connection) 
			: recordset(connection) {};

		tableContractRecordset::tableContractRecordset(const tableContractRecordset & o) {
		
			records_ = o.records_;
		
		};

		tableContractRecordset::~tableContractRecordset() {};

		tableContractRecordset & tableContractRecordset::operator=(const tableContractRecordset & o) {
		
			recordset::operator=(o);						// call the parent class =

			if (this != &o) {
			
				// copy some stuff
				records_ = o.records_;

			}

			return *this;
		
		}

		bool tableContractRecordset::open() {
		
			return true;
		
		};

		void tableContractRecordset::close() {
		

		
		};

		bool tableContractRecordset::select(const std::string & selectStr) {
		
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

		bool tableContractRecordset::insert(const ContractDetails & details) {

			std::string fieldStr, valueStr;								// the two fields to build together
			
			if (details.summary.secIdType == "SIN") {					// first manage the identifiers

				fieldStr.append("CONTRACT_ISIN,");

				INSERT_SQL_STR(valueStr, details.summary.secId)
			
				if (details.cusip != "") {
				
					fieldStr.append("CONTRACT_CUSIP,");

					INSERT_SQL_STR(valueStr, details.cusip)

				}

			} else 

			if (details.summary.secIdType == "CUSIP") {					// case is CUSIP

				fieldStr.append("CONTRACT_CUSIP,");

				INSERT_SQL_STR(valueStr, details.summary.secId)

			} else

			if (details.summary.secIdType == "SEDOL") {					// case is SEDOL

				fieldStr.append("CONTRACT_SEDOL,");

				INSERT_SQL_STR(valueStr, details.summary.secId)

				if (details.cusip != "") {

					fieldStr.append("CONTRACT_CUSIP,");

					INSERT_SQL_STR(valueStr, details.cusip)

				}

			} else

			if (details.summary.secIdType == "RIC") {					// case is RIC

				fieldStr.append("CONTRACT_RIC,");

				INSERT_SQL_STR(valueStr, details.summary.secId)

				if (details.cusip != "") {

					fieldStr.append("CONTRACT_CUSIP,");

					INSERT_SQL_STR(valueStr, details.cusip)
				}

			}
			else {													// in case nothng provided but the cusip is provided
				
				if (details.cusip != "") {

					fieldStr.append("CONTRACT_CUSIP,");

					INSERT_SQL_STR(valueStr, details.cusip)

				}

			}

			if (details.summary.conId != 0) {						// isin provided

				fieldStr.append("CONTRACT_IBID,");

				valueStr
					.append("'")
					.append(boost::lexical_cast<std::string>(
						details.summary.conId))
					.append("',");

			}

			if (details.summary.symbol != "") {						// symbol provided

				fieldStr.append("CONTRACT_SYMBOL,");

				INSERT_SQL_STR(valueStr, details.summary.symbol)

			}

			if (details.summary.secType != "") {					// sectype provided

				fieldStr.append("CONTRACT_SECTYPE,");

				INSERT_SQL_STR(valueStr, details.summary.secType)

			}

			if (details.summary.expiry != "") {						// sectype provided

				fieldStr.append("CONTRACT_EXPIRY,");

				INSERT_SQL_STR(valueStr, details.summary.expiry)

			}

			if (details.summary.strike != 0.0) {					// strike provided

				fieldStr.append("CONTRACT_STRIKE,");

				valueStr
					.append(boost::lexical_cast<std::string>(details.summary.strike))
					.append("',");

			}

			if (details.summary.right != "") {						// right provided

				fieldStr.append("CONTRACT_RIGHT,");

				std::string val("0");								// convert into unsigned int
				if (details.summary.right == "P"   ) val = "1";		// default to 0
				if (details.summary.right == "PUT" ) val = "1";
				if (details.summary.right == "C"   ) val = "2";
				if (details.summary.right == "CALL") val = "2";

				valueStr
					.append(val)
					.append("',");

			}

			if (details.summary.multiplier != "") {					// multiplier

				fieldStr.append("CONTRACT_MULTIPLIER,");

				INSERT_SQL_STR(valueStr, details.summary.multiplier)

			}

			if (details.summary.primaryExchange != "") {			// primary exchange

				fieldStr.append("CONTRACT_PRIMARY_EXCHANGE,");

				INSERT_SQL_STR(valueStr, details.summary.primaryExchange)

			}

			if (details.summary.currency != "") {					// currency

				fieldStr.append("CONTRACT_CURRENCY,");

				INSERT_SQL_STR(valueStr, details.summary.currency)

			}
			
			if (details.summary.tradingClass != "") {				// trading class

				fieldStr.append("CONTRACT_TRADING_CLASS,");

				INSERT_SQL_STR(valueStr, details.summary.tradingClass)

			}

			if (details.marketName != "") {							// market name

				fieldStr.append("CONTRACT_DETAILS_MARKET_NAME,");

				INSERT_SQL_STR(valueStr, details.marketName)

			}

			if (details.minTick != 0.0) {							// market name

				fieldStr.append("CONTRACT_DETAILS_MINTICK,");

				valueStr
					.append(boost::lexical_cast<std::string>(details.minTick))
					.append(",");

			}
			
			if (details.orderTypes != "") {							// order type string (long)

				fieldStr.append("CONTRACT_DETAILS_ORDERTYPES,");

				INSERT_SQL_STR(valueStr, details.orderTypes)

			}

			if (details.validExchanges != "") {						// valid exchanges

				fieldStr.append("CONTRACT_DETAILS_VALID_EXCHANGES,");

				INSERT_SQL_STR(valueStr, details.validExchanges)

			}
			
			if (details.priceMagnifier != 0.0) {					// price magnifier

				fieldStr.append("CONTRACT_DETAILS_PRICE_MAGNIFIER,");

				valueStr
					.append("")
					.append(boost::lexical_cast<std::string>(details.priceMagnifier))
					.append(",");

			}
			
			if (details.underConId != 0) {							// valid exchanges

				fieldStr.append("CONTRACT_DETAILS_UNDERLYING_CONTRACT_ID,");

				valueStr
					.append("'")
					.append(boost::lexical_cast<std::string>(details.underConId))
					.append("',");

			}

			if (details.longName != "") {							// valid exchanges

				fieldStr.append("CONTRACT_DETAILS_LONG_NAME,");

				INSERT_SQL_STR(valueStr, details.longName)

			}

			if (details.contractMonth != "") {						// valid exchanges

				fieldStr.append("CONTRACT_DETAILS_MONTH,");

				INSERT_SQL_STR(valueStr, details.contractMonth)

			}

			if (details.industry != "") {							// industry

				fieldStr.append("CONTRACT_DETAILS_INDUSTRY,");
				
				INSERT_SQL_STR(valueStr, details.industry)

			}

			if (details.category != "") {							// category

				fieldStr.append("CONTRACT_DETAILS_CATEGORY,");

				INSERT_SQL_STR(valueStr, details.category)

			}

			if (details.subcategory != "") {						// subcategory

				fieldStr.append("CONTRACT_DETAILS_SUBCATEGORY,");

				INSERT_SQL_STR(valueStr, details.subcategory)

			}

			if (details.timeZoneId != "") {						// timezone Id

				fieldStr.append("CONTRACT_DETAILS_TIMEZONE_ID,");

				INSERT_SQL_STR(valueStr, details.timeZoneId)

			}

			if (details.tradingHours != "") {					// timezone Id

				fieldStr.append("CONTRACT_DETAILS_TRADINGHOURS,");

				INSERT_SQL_STR(valueStr, details.tradingHours)

			}

			if (details.liquidHours != "") {						// timezone Id

				fieldStr.append("CONTRACT_DETAILS_LIQUIDHOURS,");

				INSERT_SQL_STR(valueStr, details.liquidHours)

			}

			if (details.evRule != "") {								// ev rule

				fieldStr.append("CONTRACT_DETAILS_EVRULE,");

				INSERT_SQL_STR(valueStr, details.evRule)

			}

			if (details.evMultiplier != 0.0) {						// ev multiplier

				fieldStr.append("CONTRACT_DETAILS_EVRULE_MULTIPLIER,");

				valueStr
					.append("'")
					.append(boost::lexical_cast<std::string>(details.evMultiplier))
					.append("',");

			}

			if (details.ratings != "") {						// ratings

				fieldStr.append("CONTRACT_DETAILS_BOND_RATINGS,");

				INSERT_SQL_STR(valueStr, details.ratings)

			}

			if (details.descAppend != "") {						// description

				fieldStr.append("CONTRACT_DETAILS_BOND_DESCRIPTION,");

				INSERT_SQL_STR(valueStr, details.descAppend)

			}

			if (details.bondType != "") {						// type

				fieldStr.append("CONTRACT_DETAILS_BOND_TYPE,");

				INSERT_SQL_STR(valueStr, details.bondType)

			}

			if (details.couponType != "") {						// description

				fieldStr.append("CONTRACT_DETAILS_BOND_COUPONTYPE,");

				INSERT_SQL_STR(valueStr, details.couponType)

			}

			if (details.callable == true) {						// bool callable

				fieldStr.append("CONTRACT_DETAILS_BOND_CALLABLE,");
				
				valueStr
					.append("TRUE")
					.append(",");

			} else {
			
				fieldStr.append("CONTRACT_DETAILS_BOND_CALLABLE,");

				valueStr
					.append("FALSE")
					.append(",");
			
			}

			if (details.putable == true) {						// bool putable

				fieldStr.append("CONTRACT_DETAILS_BOND_PUTABLE,");

				valueStr
					.append("TRUE")
					.append(",");

			}
			else {

				fieldStr.append("CONTRACT_DETAILS_BOND_PUTABLE,");

				valueStr
					.append("FALSE")
					.append(",");

			}

			if (details.coupon != 0.0) {						// coupon

				fieldStr.append("CONTRACT_DETAILS_BOND_COUPON,");

				valueStr
					.append("'")
					.append(boost::lexical_cast<std::string>(details.coupon))
					.append("',");

			}

			if (details.convertible == true) {					// bool convertible

				fieldStr.append("CONTRACT_DETAILS_BOND_CONVERTIBLE,");
				
				valueStr
					.append("TRUE")
					.append(",");

			}
			else {

				fieldStr.append("CONTRACT_DETAILS_BOND_CONVERTIBLE,");
				
				valueStr
					.append("FALSE")
					.append(",");

			}

			if (details.maturity != "") {						// maturity

				fieldStr.append("CONTRACT_DETAILS_BOND_MATURITY,");

				INSERT_SQL_STR(valueStr, details.maturity)

			}
			
			if (details.issueDate != "") {						// issue date

				fieldStr.append("CONTRACT_DETAILS_BOND_ISSUEDATE,");
				
				INSERT_SQL_STR(valueStr, details.issueDate)

			}
			
			if (details.nextOptionDate != "") {						// next option date

				fieldStr.append("CONTRACT_DETAILS_BOND_NEXTOPTIONDATE,");

				INSERT_SQL_STR(valueStr, details.nextOptionDate)

			}

			if (details.nextOptionType != "") {						// next option type

				fieldStr.append("CONTRACT_DETAILS_BOND_NEXTOPTIONTYPE,");

				INSERT_SQL_STR(valueStr, details.nextOptionType)

			}
			
			if (details.nextOptionPartial == true) {					// bool convertible

				fieldStr.append("CONTRACT_DETAILS_BOND_NEXTOPTIONPARTIAL,");
				
				valueStr
					.append("TRUE")
					.append(",");

			}
			else {

				fieldStr.append("CONTRACT_DETAILS_BOND_NEXTOPTIONPARTIAL,");

				valueStr
					.append("FALSE")
					.append(",");

			}

			if (details.notes != "") {						// next option type

				fieldStr.append("CONTRACT_DETAILS_BOND_NOTE,");

				INSERT_SQL_STR(valueStr, details.notes)

			}

			fieldStr.pop_back();							// erase extra characters
			valueStr.pop_back();

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

		// bulk insert
		bool tableContractRecordset::insert(const std::vector<ContractDetails> & data) {
		
			for (std::vector<ContractDetails>::const_iterator 
				It = data.cbegin(); It != data.cend(); It++)

				insert(*It);

			return true;

		}

	}
}