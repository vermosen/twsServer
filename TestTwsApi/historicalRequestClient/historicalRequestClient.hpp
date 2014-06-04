/* Copyright (C) 2013 Interactive Brokers LLC. All rights reserved. This code is subject to the terms
* and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */

#ifndef historical_request_client
#define historical_request_client

#include "EWrapper.h"

#include <memory>
#include <stdio.h> //printf()

#include <boost/shared_ptr.hpp>
#include <thOth/time/timeseries.hpp>

namespace IB {

	class EPosixClientSocket;

	struct historicalQuoteDetails {

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

	enum State {

		ST_CONNECT,
		ST_PING,
		ST_PING_ACK,
		ST_IDLE
	
	};

	class historicalRequestClient : public EWrapper {

	public:

		historicalRequestClient();
		~historicalRequestClient();

		historicalRequestClient & operator =(const historicalRequestClient &);			// assignement operator

		// accessors
		bool endOfHistoricalData() const { return endOfHistoricalData_; };				// end of data (public ?)
		bool errorForRequest() const { return errorForRequest_; };						// error
		thOth::TimeSeries<IB::historicalQuoteDetails> timeSeries() const{			// the time series
			return ts_;
		};

		void processMessages();

	public:

		bool connect(const char * host, unsigned int port, int clientId = 0);
		void disconnect() const;
		bool isConnected() const;

	private:

		void reqCurrentTime();
		void placeOrder();
		void cancelOrder();

	protected:

		// parse a date string into some dateTime
		thOth::dateTime convertDateTime(const IBString &) const;

	protected:

		bool endOfHistoricalData_;
		bool errorForRequest_;
		int marketDataType_;

		thOth::TimeSeries<IB::historicalQuoteDetails> ts_;							// timeseries object

	private:

		boost::shared_ptr<EPosixClientSocket> m_pClient;
		State m_state;
		time_t m_sleepDeadline;

	public:

		// events
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