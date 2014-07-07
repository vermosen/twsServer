#ifndef tws_recordset_hpp
#define tws_recordset_hpp

#include <string>
#include <map>

#include <stdint.h>

#include <mysql.h>

#include <thOth/time/DateTime.hpp>

// insert a string field
#define INSERT_SQL_STR(X,Y) \
	X.append("'" )          \
	 .append(Y   )          \
	 .append("',");

namespace IB {

	namespace dataBase {

		typedef uint64_t recordId;								// equivalent of a big int

		template <class T>
		class recordset {

			public:

				recordset(MYSQL *);
				recordset(const recordset &);
				
				virtual ~recordset();

				recordset & operator =(const recordset &);

				virtual bool open() = 0;
				virtual void close() = 0;

				virtual bool select(const std::string &) = 0;	// return true if the select statement is non empty

				// definition
				typedef typename std::map<recordId, T>::const_iterator const_iterator;

				// iterators
				typename const_iterator cbegin() const;
				typename const_iterator cend  () const;
				typename const_iterator begin () const { return cbegin(); };
				typename const_iterator end   () const { return cend  (); };

				recordId size() const;							// potentially large number

			protected:

				recordset() {};									// protected default ctor

				std::string convertDateTime(const thOth::dateTime &) const;
																// convert dateTime into SQL string format
				
				MYSQL     * connection_;						// connection object
				MYSQL_RES * reception_ ;

				std::map<recordId, T>    records_;				// a pile of records for data management, 
																// assumes primary key of the table is a BIGINT
		};

		template <class T>
		recordset<T>::recordset(MYSQL * connection) : connection_(connection){};
		
		template<class T>
		recordset<T>::recordset(const recordset<T> & o) {

			connection_ = o.connection_;
			reception_  = o.reception_ ;

		};
		
		template <class T>
		recordset<T>::~recordset() {};

		template <class T>
		recordset<T> & recordset<T>::operator = (const recordset<T> & o) {

			if (this != &o) {

				// copy stuff
				// DO NOT COPY FIELDS AND RECORDS HERE
				connection_ = o.connection_;
				reception_  = o.reception_ ;

			}

			return * this;

		}

		template <class T>
		std::string recordset<T>::convertDateTime(const thOth::dateTime & date) const {
		
			std::stringstream stream;
			boost::posix_time::time_facet facet;
			facet.format("%Y%m%d  %H:%M:%S");					// format '2012-02-15 00:00:00'
			stream.imbue(std::locale(std::locale::classic(), &facet));
			stream << date;

			// temp 
			std::string test = stream.str();
			//
			
			return stream.str();

		}

		template <typename T>
		inline typename recordset<T>::const_iterator
			recordset<T>::cbegin() const {

			return records_.begin();

		}

		template <class T>
		inline typename recordset<T>::const_iterator
			recordset<T>::cend() const {

			return records_.end();

		}

		template <class T>
		inline recordId recordset<T>::size() const {

			return records_.size();

		}
	
	}

}

#endif