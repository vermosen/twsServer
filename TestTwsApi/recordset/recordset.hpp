#ifndef tws_recordset_hpp
#define tws_recordset_hpp

#include <string>
#include <mysql.h>

namespace IB {

	namespace dataBase {

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

		};
	
	}

}

#endif