#ifndef table_historical_bar_recordset_hpp
#define table_historical_bar_recordset_hpp

#include <boost/lexical_cast.hpp>

#include <thOth/bar/bar.hpp>

#include "recordset/recordset.hpp"
#include "utilities/conversion/convertDatetime/convertDatetime.hpp"

namespace IB {

	namespace dataBase {

		//class barHistory { //TODO: merge with time series template

		//public:

		//	barHistory(recordId id,
		//		const thOth::bar & bar,
		//		const std::string & exchange);
		//	barHistory(const barHistory &);
		//	~barHistory() {};

		//	inline recordId instrumentId()          const { return instrumentId_; };
		//	inline std::vector<thOth::bar> & bars() const { return bars_        ; };
		//	inline std::string           exchange() const { return exchange_    ; };

		//protected:

		//	recordId instrumentId_;							// instrument db identifier
		//	std::vector<thOth::bar> bars_;
		//	std::string exchange_;

		//};

		class barRecord {

		public:

			barRecord(recordId id,
				const thOth::bar & bar,
				const std::string & exchange);
			barRecord(const barRecord &);
			~barRecord() {};

			recordId instrumentIdentifier() const { return id_      ; };
			thOth::bar                bar() const { return bar_     ; };
			std::string          exchange() const { return exchange_; };

		protected:

			recordId    id_;											// instrument db identifier
			thOth::bar  bar_;
			std::string exchange_;

		};

		class tableHistoricalBarRecordset : public recordset<barRecord> {

			public:

				 tableHistoricalBarRecordset(MYSQL *);
				 tableHistoricalBarRecordset(const tableHistoricalBarRecordset &);
				~tableHistoricalBarRecordset();

				tableHistoricalBarRecordset & operator =(const tableHistoricalBarRecordset &);

				bool selectQ(const std::string &);						// run a select statement
				bool deleteQ(const std::string &);						// delete records 
				bool insert (const barRecord   &);						// insert a bar
				

			protected:

				tableHistoricalBarRecordset() {};
				
		};
	}
}

#endif 