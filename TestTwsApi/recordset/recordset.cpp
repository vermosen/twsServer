#include "recordset/recordset.hpp"

namespace IB {

	namespace dataBase {

		recordset::recordset() {};
		recordset::recordset(const recordset & o) {};
		recordset::~recordset() {};

		recordset & recordset::operator = (const recordset & o) {
		
			if (this != &o) {
			
				// copy stuff
			
			}

			return *this;
		
		};

	}
}