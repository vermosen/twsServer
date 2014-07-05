#ifndef table_historical_bar_recordset_hpp
#define table_historical_bar_recordset_hpp

#include <boost/lexical_cast.hpp>

#include <thOth/bar/bar.hpp>

#include "recordset/recordset.hpp"

namespace IB {

	namespace dataBase {

		class tableHistoricalBarRecordset : public recordset<thOth::bar> {

			public:

				 tableHistoricalBarRecordset(MYSQL *                            );
				 tableHistoricalBarRecordset(const tableHistoricalBarRecordset &);
				~tableHistoricalBarRecordset();

				tableHistoricalBarRecordset & operator =(const tableHistoricalBarRecordset &);

				bool open();											// recordset interface
				void close();

				bool select(std::string);								// run a select statement
				bool insert(uint64_t id, const thOth::bar &);			// insert a contract detail in the database

			protected:

				tableHistoricalBarRecordset() {};
				
		};
	}
}

#endif 