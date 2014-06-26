#include "recordset/tableContractRecordset/tableContractRecordset.hpp"

namespace IB {

	namespace dataBase {
	
		tableContractRecordset::tableContractRecordset(MYSQL * connection) 
			: recordset(connection) {};

		tableContractRecordset::tableContractRecordset(const tableContractRecordset & o) {};
		tableContractRecordset::~tableContractRecordset() {};

		tableContractRecordset & tableContractRecordset::operator=(const tableContractRecordset & o) {
		
			recordset::operator=(o);						// call the parent class =

			if (this != &o) {
			
				// copy some stuff
			
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

			// need to store the results as ContractDetails
			return true;
			

		};
	
		bool tableContractRecordset::insert(const ContractDetails & details) {

			std::string insertQuery("INSERT INTO table_contract (");

			insertQuery
				.append("CONTRACT_IBID,")							// field names 
				.append("CONTRACT_SYMBOL,")
				.append("CONTRACT_SECTYPE,")
				.append("CONTRACT_EXPIRY,")
				.append("CONTRACT_STRIKE,")
				.append("CONTRACT_RIGHT,")

				.append(") VALUES(")

				.append("")											// values
				.append(")");

			return true;
		}
	}
}