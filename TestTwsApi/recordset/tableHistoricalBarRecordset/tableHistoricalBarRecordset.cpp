#include "recordset/tableHistoricalBarRecordset/tableHistoricalBarRecordset.hpp"

namespace IB {

	namespace dataBase {
	
		tableHistoricalBarRecordset::tableHistoricalBarRecordset(MYSQL * connection) 
			: recordset(connection) {};

		tableHistoricalBarRecordset::tableHistoricalBarRecordset(const tableHistoricalBarRecordset & o) {
		
			records_ = o.records_;
		
		};

		tableHistoricalBarRecordset::~tableHistoricalBarRecordset() {};

		tableHistoricalBarRecordset & tableHistoricalBarRecordset::operator=(const tableHistoricalBarRecordset & o) {
		
			recordset::operator=(o);						// call the parent class =

			if (this != &o) {
			
				// copy some stuff
				records_ = o.records_;

			}

			return *this;
		
		}

		bool tableHistoricalBarRecordset::open() {
		
			return true;
		
		};

		void tableHistoricalBarRecordset::close() {
		

		
		};

		bool tableHistoricalBarRecordset::select(std::string selectStr) {
		
			mysql_query(												// query to run
				connection_,
				selectStr.c_str());

			reception_ = mysql_store_result(connection_);
		
			if (!reception_)											// sql statement failed
				return false;

			if (reception_->row_count == 0)								// no record
				return false;

			// local copy in records_
			//records_.insert();
			return true;

		};

		bool tableHistoricalBarRecordset::insert(const barRecord & rec) {

			std::string fieldStr, valueStr;								// the two fields to build together

			fieldStr.append("CONTRACT_ID,");							// contract id

			valueStr
				.append(boost::lexical_cast<std::string>(rec.id_))
				.append(",");

			fieldStr.append("BAR_START,");								// barStart
			
			// TODO: conversion to format
			// SELECT * FROM util_audit
			// WHERE DATED BETWEEN '2012-02-15 00:00:00' AND '2012-03-31 00:00:00';

			valueStr
				.append(boost::lexical_cast<std::string>(rec.bar_.barStart()))
				.append(",");
			
			fieldStr.append("BAR_END,");								// barEnd

			valueStr
				.append(boost::lexical_cast<std::string>(rec.bar_.barEnd()))
				.append(",");

			fieldStr.pop_back();							// erase extra characters
			valueStr.pop_back();

			std::string insertStatement("INSERT INTO table_contract (");
			insertStatement
				.append(fieldStr)
				.append(") VALUES (")
				.append(valueStr)
				.append(")");

			if (mysql_query(connection_, insertStatement.c_str()) != 0)		// throw on an error
				throw std::exception(mysql_error(connection_));

			return true;													// return true otherwise

		}
	}
}