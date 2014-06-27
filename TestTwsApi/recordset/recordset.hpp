#ifndef tws_recordset_hpp
#define tws_recordset_hpp

#include <string>
#include <map>
#include <stdint.h>

#include <mysql.h>

namespace IB {

	namespace dataBase {

		template <class T>
		class recordset {

			public:

				recordset(MYSQL *);
				recordset(const recordset &);
				
				virtual ~recordset();

				recordset & operator =(const recordset &);

				virtual bool open() = 0;
				virtual void close() = 0;

				virtual bool select(std::string) = 0;		// return true if the select statement is non empty

			protected:

				recordset() {};								// protected default ctor
				
				MYSQL     * connection_;					// connection object
				MYSQL_RES * reception_ ;

				std::map<uint64_t, T>    records_;			// a pile of records for data management, 
															// assumes primary key is a BIGINT

		};

		template <class T>
		recordset<T>::recordset(MYSQL * connection) : connection_(connection){};
		
		template<class T>
		recordset<T>::recordset(const recordset<T> & o) {

			connection_ = o.connection_;
			reception_ = o.reception_;

		};
		
		template <class T>
		recordset<T>::~recordset() {};

		template <class T>
		recordset<T> & recordset<T>::operator = (const recordset<T> & o) {

			if (this != &o) {

				// copy stuff
				// DO NOT COPY FIELDS AND RECORDS HERE
				connection_ = o.connection_;
				reception_ = o.reception_;

			}

			return * this;

		}
	
	}

}

#endif