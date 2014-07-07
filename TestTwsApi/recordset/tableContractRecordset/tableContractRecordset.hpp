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

				bool select(const std::string &);						// run a select statement
				bool insert(const ContractDetails &);					// insert a contract detail in the database
				bool insert(const std::vector<ContractDetails> &);		// insert a contract detail in the database

			protected:

				tableContractRecordset() {};
				
		};
	}
}

#endif 