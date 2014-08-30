#ifndef tws_recordset_hpp
#define tws_recordset_hpp

#include <string>
#include <map>

#include <stdint.h>

#include <mysql.h>

#include <thOth/time/DateTime.hpp>

#include "recordset/recordsetException/recordsetException.hpp"

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

			typedef typename std::map<recordId, T>::const_iterator const_iterator;

			public:

				recordset(MYSQL *);								// ctor
				recordset(const recordset &);					// copy ctor

				virtual ~recordset();							// virtual destructor

				recordset & operator =(const recordset &);

				// database methods
				virtual bool select(const std::string &) = 0;	// return true if the select statement is non empty
				virtual bool insert(const T			  &) = 0;	// insert method
				bool insert(const std::vector<T> &);			// bulk insert

				// iterators
				typename const_iterator cbegin() const;
				typename const_iterator cend  () const;
				typename const_iterator begin () const { return cbegin(); };
				typename const_iterator end   () const { return cend  (); };

				recordId size() const;

			protected:

				recordset() {};									// protected default ctor

				MYSQL     * connection_;						// connection objects
				MYSQL_RES * reception_ ;

				std::map<recordId, T> records_;					// a pile of records for data management, 
																// assumes the primary key of every table is a BIGINT
																// otherwise some internal key will be allocated
		};

		// implementation
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
				// DO NOT DUPLICATE FIELDS AND RECORDS HERE
				connection_ = o.connection_;
				reception_  = o.reception_ ;

			}

			return * this;

		}

		// bulk insert
		template <typename T>
		bool recordset<T>::insert(const std::vector<T> & data) {
		
			for (std::vector<T>::const_iterator
				It = data.cbegin(); It != data.cend(); It++)

				insert(*It);

			return true;
		
		};

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