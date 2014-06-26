#ifndef table_contract_recordset_hpp
#define table_contract_recordset_hpp

#include "recordset/recordset.hpp"
#include <contract.h>

namespace IB {

	namespace dataBase {

		class tableContractRecordset : public recordset {

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

		};

	}
}

#endif 