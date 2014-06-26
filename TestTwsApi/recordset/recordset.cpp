#include "recordset/recordset.hpp"

namespace IB {

	namespace dataBase {

		recordset::recordset(MYSQL * connection) : connection_(connection){};
		recordset::recordset(const recordset & o) {
		
			connection_ = o.connection_;
		
		};
		recordset::~recordset() {};

		recordset & recordset::operator = (const recordset & o) {
		
			if (this != &o) {
			
				// copy stuff
				connection_ = o.connection_;
			
			}

			return *this;
		
		};

	}
}