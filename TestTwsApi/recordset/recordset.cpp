#include "recordset/recordset.hpp"

namespace IB {

	namespace dataBase {

		recordset::recordset<class T>(MYSQL * connection) : connection_(connection){};
		recordset<T>::recordset(const recordset & o) {
		
			connection_ = o.connection_;
		
		};
		recordset<T>::~recordset() {};

		recordset & recordset::operator = (const recordset & o) {
		
			if (this != &o) {
			
				// copy stuff
				connection_ = o.connection_;
			
			}

			return *this;
		
		};

	}
}