#ifndef tws_recordset_hpp
#define tws_recordset_hpp

namespace IB {

	namespace dataBase {

		class recordset {

			public:

				recordset();
				recordset(const recordset &);
				~recordset();

				recordset & operator =(const recordset &);

				virtual bool Open();
				virtual void Close();

				virtual void connect() const = 0;
				virtual void selectQuery(std::string &)

		};
	
	}

}

#endif