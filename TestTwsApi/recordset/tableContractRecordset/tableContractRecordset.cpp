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

		bool tableContractRecordset::select(std::string selectStr) {
		
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

			std::string insertStatement("INSERT INTO table_contract (");

			for (std::vector<std::string>::const_iterator				// iterate over the fields
				It = fields_.cbegin(); It != fields_.cend(); It++) {
			
				insertStatement
					.append(*It)
					.append(",");
			
			}
			
			insertStatement.pop_back();									// removes the last column
			
			insertStatement
				.append(") VALUES ('");

			std::string cusip;											// the identifiers
			std::string isin;
			std::string sedol;
			std::string ric;

			if (details.summary.secIdType == "SIN")						// isin provided
				isin = details.summary.secIdType;
			
			if (details.summary.secIdType == "CUSIP")					// cusip provided
				cusip = details.summary.secIdType;
			
			if (details.summary.secIdType == "SEDOL")					// sedol provided
				sedol = details.summary.secIdType;
			
			if (details.summary.secIdType == "RIC")						// ric provided
				ric = details.summary.secIdType;

			if (!details.cusip.empty()) cusip = details.cusip;			// if it is a bond, overwrite

			insertStatement												// values
				.append(boost::lexical_cast<std::string>(details.summary.conId))
				.append("', '")
				.append(details.summary.symbol)
				.append("', '")
				.append(details.summary.secType)
				.append("', '")
				.append(details.summary.expiry)
				.append("', '")
				.append(boost::lexical_cast<std::string>(details.summary.strike))
				.append("', '")
				.append(details.summary.right)
				.append("', '")
				.append(details.summary.multiplier)
				.append("', '")
				.append(details.summary.primaryExchange)
				.append("', '")
				.append(details.summary.currency)
				.append("', '")
				.append(details.summary.tradingClass)
				.append("', '")
				.append(isin)							// have to select 1 of 3
				.append("', '")
				.append(cusip)
				.append("', '")
				.append(sedol)
				.append("', '")
				.append(ric)
				.append("', '")
				.append(details.marketName)
				.append("', '")
				.append(boost::lexical_cast<std::string>(details.minTick))
				.append("', '")
				.append(details.orderTypes)
				.append("', '")
				.append(details.validExchanges)
				.append("', '")
				.append(boost::lexical_cast<std::string>(details.priceMagnifier))
				.append("', '")
				.append(boost::lexical_cast<std::string>(details.underConId))
				.append("', '")
				.append(details.longName)
				.append("', '")
				.append(details.contractMonth)
				.append("', '")
				.append(details.industry)
				.append("', '")
				.append(details.category)
				.append("', '")
				.append(details.subcategory)
				.append("', '")
				.append(details.timeZoneId)
				.append("', '")
				.append(details.tradingHours)
				.append("', '")
				.append(details.liquidHours)
				.append("', '")
				.append(details.evRule)
				.append("', '")
				.append(boost::lexical_cast<std::string>(details.evMultiplier))
				.append("', '")
				.append(details.ratings)
				.append("', '")
				.append(details.descAppend)
				.append("', '")
				.append(details.bondType)
				.append("', '")
				.append(details.couponType)
				.append("', '")
				.append(boost::lexical_cast<std::string>((int)details.callable))
				.append("', '")
				.append(boost::lexical_cast<std::string>((int)details.putable))
				.append("', '")
				.append(boost::lexical_cast<std::string>(details.coupon))
				.append("', '")
				.append(boost::lexical_cast<std::string>((int)details.convertible))
				.append("', '")
				.append(details.maturity)
				.append("', '")
				.append(details.issueDate)
				.append("', '")
				.append(details.nextOptionDate)
				.append("', '")
				.append(details.nextOptionType)
				.append("', '")
				.append(boost::lexical_cast<std::string>((int)details.nextOptionPartial))
				.append("', '")
				.append(details.notes)
				.append("')");

			if (mysql_query(connection_, insertStatement.c_str()) != 0)		// throw on an error
				throw std::exception(mysql_error(connection_));

			return true;													// return true otherwise

		}
	}
}