#ifndef table_historical_bar_recordset_hpp
#define table_historical_bar_recordset_hpp

#include <boost/lexical_cast.hpp>

#include <thOth/bar/bar.hpp>

#include "recordset/recordset.hpp"

namespace IB {

	namespace dataBase {

		struct barRecord {

			recordId    id_      ;
			thOth::bar  bar_     ;
			std::string exchange_;

		};

		class tableHistoricalBarRecordset : public recordset<thOth::bar> {

			public:

				 tableHistoricalBarRecordset(MYSQL *                            );
				 tableHistoricalBarRecordset(const tableHistoricalBarRecordset &);
				~tableHistoricalBarRecordset();

				tableHistoricalBarRecordset & operator =(const tableHistoricalBarRecordset &);

				bool open();											// recordset interface
				void close();

				bool select(const std::string &);						// run a select statement
				bool insert(const barRecord &  );						// insert an historical bar

			protected:

				tableHistoricalBarRecordset() {};
				
		};
	}
}

#endif 