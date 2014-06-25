#ifndef table_contract_recordset_hpp
#define table_contract_recordset_hpp

#include "recordset/recordset.hpp"

namespace IB {

	namespace dataBase {

		class tableContractRecordset : public recordset {

			public:

				tableContractRecordset();
				tableContractRecordset(const tableContractRecordset &);
				~tableContractRecordset();

				tableContractRecordset & operator =(const tableContractRecordset &);

				// recordset interface
				void connect() const {};

		};

	}
}

#endif 