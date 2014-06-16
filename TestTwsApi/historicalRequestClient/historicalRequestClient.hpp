/* Copyright (C) 2013 Interactive Brokers LLC. All rights reserved. This code is subject to the terms
* and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */

#ifndef historical_request_client_hpp
#define historical_request_client_hpp

#include "EWrapper.h"																	// TWS components
#include "contract.h"																	

#include "utilities/factory/barSizeFactory/barSizeFactory.hpp"							// factories
#include "utilities/factory/dataTypeFactory/dataTypeFactory.hpp"
#include "utilities/factory/dataDurationFactory/dataDurationFactory.hpp"

//#include <memory>
#include <stdio.h>																		// printf()

#include <boost/shared_ptr.hpp>
#include <thOth/time/DateTime.hpp>
#include <thOth/time/timeseries.hpp>
#include <thOth/pattern/observable.hpp>

namespace IB {

	class EPosixClientSocket;
	struct Contract;

	struct historicalQuoteDetails {														// data structure for historical request

		TickerId id_;
		double open_;
		double high_;
		double low_;
		double close_;
		int volume_;
		int barCount_;
		double WAP_;
		int hasGaps_;

	};

	class historicalRequestClient : public EWrapper, thOth::observable {

	private:

		enum state {																		// client states

			ST_CONNECT,
			ST_REQUEST,
			ST_REQUEST_ACK,
			ST_PING,
			ST_PING_ACK,
			ST_IDLE

		};

	private:																			// private default ctor, cc ctors and assignement
		historicalRequestClient() {};													// TODO: turn it into a singleton
		historicalRequestClient(const historicalRequestClient &) {};

		historicalRequestClient & operator =(const historicalRequestClient &);

	public:

		historicalRequestClient(const Contract &,										// public ctor
								const thOth::dateTime &,
								const barSize bs = barSize::oneSecond,
								const int lenght = 1,
								const dataDuration dur = dataDuration::day,
								const dataType = dataType::trade);

		~historicalRequestClient();														// destructor

		// accessors
		bool endOfHistoricalData() const { return endOfHistoricalData_; };				// end of data (public ?)
		bool errorForRequest() const { return errorForRequest_; };						// error
		
		thOth::TimeSeries<historicalQuoteDetails> timeSeries() const{					// the time series
			return ts_;
		};

		void processMessages();

	public:

		bool connect(const char * host, unsigned int port, int clientId = 0);
		void disconnect() const;
		bool isConnected() const;

	private:

		void reqCurrentTime();

	protected:

		thOth::dateTime convertDateTime(const IBString &) const;					// parse a date string into some dateTime
		IBString convertDateTime(const thOth::dateTime &) const;
		void requestHistoricalData();												// request data

		bool IsEndOfHistoricalData(const IBString& Date) {							// check if historical data is finished

			endOfHistoricalData_ = 1 + strncmp((const char*)Date.data(), "finished", 8);
			return endOfHistoricalData_;
		
		}

	private:

		bool endOfHistoricalData_;													// indicate whether the file has been read
		bool errorForRequest_;														// error on the request
		int marketDataType_;														// market data type
		
		Contract contract_;															// the contract definition
		thOth::dateTime endDate_;													// the end date
		int length_;																// lenght of the period
		barSize barSize_;															// bar size
		dataDuration dataDuration_;													// data duration
		dataType dataType_;															// data Type

		thOth::TimeSeries<historicalQuoteDetails> ts_;								// timeseries object

		boost::shared_ptr<EPosixClientSocket> m_pClient;							// posix client
		state m_state;																// current state
		time_t m_sleepDeadline;														// sleep deadline

	public:

		// Ewrapper implentation
		void tickPrice(TickerId tickerId, TickType field, double price, int canAutoExecute);
		void tickSize(TickerId tickerId, TickType field, int size);
		void tickOptionComputation(TickerId tickerId, TickType tickType, double impliedVol, double delta,
			double optPrice, double pvDividend, double gamma, double vega, double theta, double undPrice);
		void tickGeneric(TickerId tickerId, TickType tickType, double value);
		void tickString(TickerId tickerId, TickType tickType, const IBString& value);
		void tickEFP(TickerId tickerId, TickType tickType, double basisPoints, const IBString& formattedBasisPoints,
			double totalDividends, int holdDays, const IBString& futureExpiry, double dividendImpact, double dividendsToExpiry);
		void orderStatus(OrderId orderId, const IBString &status, int filled,
			int remaining, double avgFillPrice, int permId, int parentId,
			double lastFillPrice, int clientId, const IBString& whyHeld);
		void openOrder(OrderId orderId, const Contract&, const Order&, const OrderState&);
		void openOrderEnd();
		void winError(const IBString &str, int lastError);
		void connectionClosed();
		void updateAccountValue(const IBString& key, const IBString& val,
			const IBString& currency, const IBString& accountName);
		void updatePortfolio(const Contract& contract, int position,
			double marketPrice, double marketValue, double averageCost,
			double unrealizedPNL, double realizedPNL, const IBString& accountName);
		void updateAccountTime(const IBString& timeStamp);
		void accountDownloadEnd(const IBString& accountName);
		void nextValidId(OrderId orderId);
		void contractDetails(int reqId, const ContractDetails& contractDetails);
		void bondContractDetails(int reqId, const ContractDetails& contractDetails);
		void contractDetailsEnd(int reqId);
		void execDetails(int reqId, const Contract& contract, const Execution& execution);
		void execDetailsEnd(int reqId);
		void error(const int id, const int errorCode, const IBString errorString);
		void updateMktDepth(TickerId id, int position, int operation, int side,
			double price, int size);
		void updateMktDepthL2(TickerId id, int position, IBString marketMaker, int operation,
			int side, double price, int size);
		void updateNewsBulletin(int msgId, int msgType, const IBString& newsMessage, const IBString& originExch);
		void managedAccounts(const IBString& accountsList);
		void receiveFA(faDataType pFaDataType, const IBString& cxml);
		void historicalData(TickerId reqId, const IBString& date, double open, double high,
			double low, double close, int volume, int barCount, double WAP, int hasGaps);
		void scannerParameters(const IBString &xml);
		void scannerData(int reqId, int rank, const ContractDetails &contractDetails,
			const IBString &distance, const IBString &benchmark, const IBString &projection,
			const IBString &legsStr);
		void scannerDataEnd(int reqId);
		void realtimeBar(TickerId reqId, long time, double open, double high, double low, double close,
			long volume, double wap, int count);
		void currentTime(long time);
		void fundamentalData(TickerId reqId, const IBString& data);
		void deltaNeutralValidation(int reqId, const UnderComp& underComp);
		void tickSnapshotEnd(int reqId);
		void marketDataType(TickerId reqId, int marketDataType);
		void commissionReport(const CommissionReport& commissionReport);
		void position(const IBString& account, const Contract& contract, int position, double avgCost);
		void positionEnd();
		void accountSummary(int reqId, const IBString& account, const IBString& tag, const IBString& value, const IBString& curency);
		void accountSummaryEnd(int reqId);

	};

}
#endif