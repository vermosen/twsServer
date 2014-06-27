#ifndef table_contract_recordset_hpp
#define table_contract_recordset_hpp

#include <contract.h>

#include "recordset/recordset.hpp"

#include <boost/lexical_cast.hpp>

namespace IB {

	namespace dataBase {

		class tableContractRecordset : public recordset<ContractDetails> {

			public:

				tableContractRecordset(MYSQL *);
				tableContractRecordset(const tableContractRecordset &);
				~tableContractRecordset();

				tableContractRecordset & operator =(const tableContractRecordset &);

				bool open();											// recordset interface
				void close();

				bool select(std::string);								// run a select statement
				bool insert(const IB::ContractDetails &);				// insert a contract detail in the database

			protected:

				tableContractRecordset() {};
				const std::vector<std::string> fields_ = std::vector<std::string> {

					std::string("CONTRACT_IBID"),
					std::string("CONTRACT_SYMBOL"),
					std::string("CONTRACT_SECTYPE"),
					std::string("CONTRACT_EXPIRY"),
					std::string("CONTRACT_STRIKE"),
					std::string("CONTRACT_RIGHT"),
					std::string("CONTRACT_MULTIPLIER"),
					std::string("CONTRACT_PRIMARY_EXCHANGE"),
					std::string("CONTRACT_CURRENCY"),
					std::string("CONTRACT_TRADING_CLASS"),
					std::string("CONTRACT_ISIN"),
					std::string("CONTRACT_CUSIP"),
					std::string("CONTRACT_SEDOL"),
					std::string("CONTRACT_RIC"),
					std::string("CONTRACT_DETAILS_MARKET_NAME"),
					std::string("CONTRACT_DETAILS_MINTICK"),
					std::string("CONTRACT_DETAILS_ORDERTYPES"),
					std::string("CONTRACT_DETAILS_VALID_EXCHANGES"),
					std::string("CONTRACT_DETAILS_PRICE_MAGNIFIER"),
					std::string("CONTRACT_DETAILS_UNDERLYING_CONTRACT_ID"),
					std::string("CONTRACT_DETAILS_LONG_NAME"),
					std::string("CONTRACT_DETAILS_MONTH"),
					std::string("CONTRACT_DETAILS_INDUSTRY"),
					std::string("CONTRACT_DETAILS_CATEGORY"),
					std::string("CONTRACT_DETAILS_SUBCATEGORY"),
					std::string("CONTRACT_DETAILS_TIMEZONE_ID"),
					std::string("CONTRACT_DETAILS_TRADINGHOURS"),
					std::string("CONTRACT_DETAILS_LIQUIDHOURS"),
					std::string("CONTRACT_DETAILS_EVRULE"),
					std::string("CONTRACT_DETAILS_EVRULE_MULTIPLIER"),
					std::string("CONTRACT_DETAILS_BOND_RATINGS"),
					std::string("CONTRACT_DETAILS_BOND_DESCRIPTION"),
					std::string("CONTRACT_DETAILS_BOND_TYPE"),
					std::string("CONTRACT_DETAILS_BOND_COUPONTYPE"),
					std::string("CONTRACT_DETAILS_BOND_CALLABLE"),
					std::string("CONTRACT_DETAILS_BOND_PUTABLE"),
					std::string("CONTRACT_DETAILS_BOND_COUPON"),
					std::string("CONTRACT_DETAILS_BOND_CONVERTIBLE"),
					std::string("CONTRACT_DETAILS_BOND_MATURITY"),
					std::string("CONTRACT_DETAILS_BOND_ISSUEDATE"),
					std::string("CONTRACT_DETAILS_BOND_NEXTOPTIONDATE"),
					std::string("CONTRACT_DETAILS_BOND_NEXTOPTIONTYPE"),
					std::string("CONTRACT_DETAILS_BOND_NEXTOPTIONPARTIAL"),
					std::string("CONTRACT_DETAILS_BOND_NOTE")

				};
		};
	}
}

#endif 