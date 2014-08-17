#ifndef table_historical_bar_recordset_hpp
#define table_historical_bar_recordset_hpp

#include <boost/lexical_cast.hpp>

#include <thOth/bar/bar.hpp>

#include "recordset/recordset.hpp"
#include "utilities/conversion/convertDatetime/convertDatetime.hpp"

namespace IB {

	namespace dataBase {

		class barRecord {

			public:

				barRecord(recordId id, 
						  const thOth::bar & bar,
						  const std::string & exchange);
				barRecord(const barRecord &);
				~barRecord() {};

				recordId instrumentIdentifier() const { return id_; };
				thOth::bar bar() const { return bar_; };
				std::string exchange() const { return exchange_; };

			protected:

				recordId    id_      ;
				thOth::bar  bar_     ;
				std::string exchange_;

		};

		class tableHistoricalBarRecordset : public recordset<barRecord> {

			public:

				 tableHistoricalBarRecordset(MYSQL *                            );
				 tableHistoricalBarRecordset(const tableHistoricalBarRecordset &);
				~tableHistoricalBarRecordset();

				tableHistoricalBarRecordset & operator =(const tableHistoricalBarRecordset &);

				bool select(const std::string &);						// run a select statement
				bool insert(const barRecord &  );						// insert an historical bar

			protected:

				tableHistoricalBarRecordset() {};
				
		};
	}
}

#endif 