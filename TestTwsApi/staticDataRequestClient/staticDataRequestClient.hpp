/* Copyright (C) 2013 Interactive Brokers LLC. All rights reserved. This code is subject to the terms
* and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */

#ifndef static_data_request_client_hpp
#define static_data_request_client_hpp

#include <EWrapper.h>																	// TWS components
#include <contract.h>																	

//#include <memory>
#include <stdio.h>																		// printf()

#include <boost/shared_ptr.hpp>
#include <thOth/time/DateTime.hpp>
#include <thOth/time/timeseries.hpp>
#include <thOth/pattern/observable.hpp>

namespace IB {

	class EPosixClientSocket;
	struct Contract;

	class staticDataRequestClient : public EWrapper, thOth::observable {

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
		staticDataRequestClient() {};													// TODO: turn it into a singleton
		staticDataRequestClient(const staticDataRequestClient &) {};

		staticDataRequestClient & operator =(const staticDataRequestClient &);

	public:

		staticDataRequestClient(const Contract &);

		~staticDataRequestClient();														// destructor

		// accessors
		bool endOfStaticData() const { return endOfStaticData_; };						// end of data (public ?)
		bool errorForRequest() const { return errorForRequest_; };						// error
		
		// accessors
		Contract        contract()        const { return contract_       ; };			// contract
		ContractDetails contractDetails() const { return contractDetails_; };			// contract detailss

		void processMessages();

	public:

		bool connect(const char * host, unsigned int port, int clientId = 0);
		void disconnect() const;
		bool isConnected() const;

	private:

		void reqCurrentTime();

	protected:

		thOth::dateTime convertDateTime(const IBString &       ) const;				// parse a date string into some dateTime
		IBString        convertDateTime(const thOth::dateTime &) const;
		void requestStaticData();													// request static data

		// maybe useless
		bool IsEndOfStaticData(const IBString& Date) {								// check if static request has been achieve

			endOfStaticData_ = 1 + strncmp((const char*)Date.data(), "finished", 8);	// todo: check for request achivement
			return endOfStaticData_;
		
		}

	private:

		bool endOfStaticData_;														// indicate whether the file has been read
		bool errorForRequest_;														// error on the request
		int marketDataType_;														// market data type
		
		Contract contract_;															// the initial contract definition
		ContractDetails contractDetails_;											// the contract details returned

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