/* Copyright (C) 2013 Interactive Brokers LLC. All rights reserved. This code is subject to the terms
* and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */

#include "staticDataRequestClient/staticDataRequestClient.hpp"
#include "EPosixClientSocket.h"

/* In this example we just include the platform header to have select(). In real
life you should include the needed headers from your system. */
#include "EPosixClientSocketPlatform.h"
#include "Contract.h"
#include "Order.h"

#include <time.h>

#include <thOth/time/timeseries.hpp>

#ifndef _MSC_VER
#include <sys/time.h>
#endif

#if defined __INTEL_COMPILER
# pragma warning (disable:869)
#elif defined __GNUC__
# pragma GCC diagnostic ignored "-Wswitch"
# pragma GCC diagnostic ignored "-Wunused-parameter"
#endif  /* __INTEL_COMPILER */

namespace IB {

	const int PING_DEADLINE = 2;							// seconds
	const int SLEEP_BETWEEN_PINGS = 30;						// seconds

	///////////////////////////////////////////////////////////
	// member funcs
	staticDataRequestClient::staticDataRequestClient(const Contract & ct)

		: m_pClient(new EPosixClientSocket(this)),
		  m_state(ST_CONNECT),
		  m_sleepDeadline(0),
		  contract_(ct),
		  id_(0) {};

	staticDataRequestClient::~staticDataRequestClient() {}

	staticDataRequestClient & staticDataRequestClient::operator = (const staticDataRequestClient & o) {
	
		if (this != &o) {									// member copy
		
			endOfStaticData_ = o.endOfStaticData_;
			errorForRequest_ = o.errorForRequest_;
			marketDataType_  = o.marketDataType_ ;

			contract_        = o.contract_       ;
			contractDetails_ = o.contractDetails_;

			m_pClient        = o.m_pClient       ;
			m_state          = o.m_state         ;
			m_sleepDeadline  = o.m_sleepDeadline ;
			
			id_ = 0;										// do not copy the id, set the new one to 0 instead

		}
	
		return *this;
	
	};

	bool staticDataRequestClient::connect(
		const char *host, 
		unsigned int port, 
		int clientId) {
						
		// TODO: test how clientId has to be set or not here
		bool bRes = m_pClient->eConnect2(					// trying to connect
			host, port, clientId);
		return bRes;

	}

	void staticDataRequestClient::disconnect() const {

		m_pClient->eDisconnect();

	}

	bool staticDataRequestClient::isConnected() const {

		return m_pClient->isConnected();

	}

	void staticDataRequestClient::requestStaticData() {
	
		requestId();										// generates a new id
		m_pClient->reqContractDetails(						// call the corresponding EClientSocketBase method
			id_,
			contract_);
		
		m_state = ST_REQUEST_ACK;
	
	}

	void staticDataRequestClient::processMessages() {

		fd_set readSet, writeSet;

		struct timeval tval;
		tval.tv_usec = 0;
		tval.tv_sec = 0;

		time_t now = time(NULL);

		switch (m_state) {

		case ST_REQUEST:
			requestStaticData();

		case ST_REQUEST_ACK:
			break;

		case ST_PING:
			reqCurrentTime();
			break;

		case ST_PING_ACK:
			if (m_sleepDeadline < now) {

				disconnect();
				return;
			
			}
			
			break;
		
		case ST_IDLE:
			if (m_sleepDeadline < now) {

				m_state = ST_PING;
				return;
			
			}
			break;
		}
		
		if (m_sleepDeadline > 0)							// initialize timeout with m_sleepDeadline - now
			tval.tv_sec = static_cast<long>(m_sleepDeadline - now);

		if (m_pClient->fd() >= 0) {

			FD_ZERO(&readSet);
			writeSet = readSet;

			FD_SET(m_pClient->fd(), &readSet);

			if (!m_pClient->isOutBufferEmpty())
				FD_SET(m_pClient->fd(), &writeSet);

			int ret = select(m_pClient->fd() + 1, &readSet, &writeSet, NULL, &tval);

			if (ret == 0)									// timeout
				return;

			if (ret < 0) {									// error
				disconnect();
				return;
			}

			if (FD_ISSET(m_pClient->fd(), &writeSet))				
				m_pClient->onSend();						// socket is ready for writing

			if (m_pClient->fd() < 0)
				return;

			if (FD_ISSET(m_pClient->fd(), &readSet))				
				m_pClient->onReceive();						// socket is ready for reading

		}
	}

	//////////////////////////////////////////////////////////////////
	// methods
	void staticDataRequestClient::reqCurrentTime()
	{

		m_sleepDeadline = time(NULL) + PING_DEADLINE;		// set ping deadline to "now + n seconds"
		m_state = ST_PING_ACK;
		m_pClient->reqCurrentTime();

	}

	void staticDataRequestClient::nextValidId(OrderId orderId) {
	
		m_state = ST_REQUEST;
	
	}

	void staticDataRequestClient::currentTime(long time) {

		if (m_state == ST_PING_ACK) {
			
			time_t t = (time_t)time;
			struct tm * timeinfo = localtime(&t);
			time_t now = ::time(NULL);
			m_sleepDeadline = now + SLEEP_BETWEEN_PINGS;
			m_state = ST_IDLE;

		}
	}

	void staticDataRequestClient::error(
		const int id, 
		const int errorCode, 
		const IBString errorString) {

		if (id == -1 && errorCode == 1100)					// if "Connectivity between IB and TWS has been lost"
			disconnect();
		else if (errorCode == 200)								// contract has no match
			disconnect();
		else
			std::cout
				<< "request information: "
				<< errorString
				<< std::endl;

	}

	void staticDataRequestClient::contractDetails(			// eWrapper implementation 
		int reqId,
		const ContractDetails& contractDetails) {

		endOfStaticData_ = true;
		disconnect();
		contractDetails_ = contractDetails;
		notifyObservers();
		return;

	}

	void staticDataRequestClient::contractDetailsEnd(int reqId) {
	
		// no idea of it's usage
		std::cout << "contractDetailsEnd has been called" << std::endl;
	
	}

	void staticDataRequestClient::tickPrice(TickerId tickerId, TickType field, double price, int canAutoExecute) {}
	void staticDataRequestClient::tickSize(TickerId tickerId, TickType field, int size) {}
	void staticDataRequestClient::tickOptionComputation(
		TickerId tickerId, TickType tickType, double impliedVol, double delta,
		double optPrice, double pvDividend,
		double gamma, double vega, double theta, double undPrice) {}
	void staticDataRequestClient::tickGeneric(
		TickerId tickerId, TickType tickType, double value) {}
	void staticDataRequestClient::tickString(
		TickerId tickerId, TickType tickType, const IBString& value) {}
	void staticDataRequestClient::tickEFP(
		TickerId tickerId, TickType tickType, double basisPoints, const IBString& formattedBasisPoints,
		double totalDividends, int holdDays, const IBString& futureExpiry, double dividendImpact, double dividendsToExpiry) {}
	void staticDataRequestClient::openOrder(OrderId orderId, const Contract&, const Order&, const OrderState& ostate) {}
	void staticDataRequestClient::openOrderEnd() {}
	void staticDataRequestClient::winError(const IBString &str, int lastError) {}
	void staticDataRequestClient::connectionClosed() {}
	void staticDataRequestClient::updateAccountValue(const IBString& key, const IBString& val,
		const IBString& currency, const IBString& accountName) {}
	void staticDataRequestClient::updatePortfolio(const Contract& contract, int position,
		double marketPrice, double marketValue, double averageCost,
		double unrealizedPNL, double realizedPNL, const IBString& accountName){}
	void staticDataRequestClient::updateAccountTime(const IBString& timeStamp) {}
	void staticDataRequestClient::accountDownloadEnd(const IBString& accountName) {}
	void staticDataRequestClient::bondContractDetails(int reqId, const ContractDetails& contractDetails) {}
	void staticDataRequestClient::execDetails(int reqId, const Contract& contract, const Execution& execution) {}
	void staticDataRequestClient::execDetailsEnd(int reqId) {}

	void staticDataRequestClient::updateMktDepth(TickerId id, int position, int operation, int side,
		double price, int size) {}
	void staticDataRequestClient::updateMktDepthL2(TickerId id, int position, IBString marketMaker, int operation,
		int side, double price, int size) {}
	void staticDataRequestClient::updateNewsBulletin(int msgId, int msgType, const IBString& newsMessage, const IBString& originExch) {}
	void staticDataRequestClient::managedAccounts(const IBString& accountsList) {}
	void staticDataRequestClient::receiveFA(faDataType pFaDataType, const IBString& cxml) {}
	void staticDataRequestClient::scannerParameters(const IBString &xml) {}
	void staticDataRequestClient::scannerData(int reqId, int rank, const ContractDetails &contractDetails,
		const IBString &distance, const IBString &benchmark, const IBString &projection,
		const IBString &legsStr) {}
	void staticDataRequestClient::scannerDataEnd(int reqId) {}
	void staticDataRequestClient::realtimeBar(TickerId reqId, long time, double open, double high, double low, double close,
		long volume, double wap, int count) {}
	void staticDataRequestClient::fundamentalData(TickerId reqId, const IBString& data) {}
	void staticDataRequestClient::deltaNeutralValidation(int reqId, const UnderComp& underComp) {}
	void staticDataRequestClient::tickSnapshotEnd(int reqId) {}
	void staticDataRequestClient::marketDataType(TickerId reqId, int marketDataType) {}
	void staticDataRequestClient::commissionReport(const CommissionReport& commissionReport) {}
	void staticDataRequestClient::position(const IBString& account, const Contract& contract, int position, double avgCost) {}
	void staticDataRequestClient::positionEnd() {}
	void staticDataRequestClient::accountSummary(int reqId, const IBString& account, const IBString& tag, const IBString& value, const IBString& curency) {}
	void staticDataRequestClient::accountSummaryEnd(int reqId) {}
	void staticDataRequestClient::orderStatus(
		OrderId orderId, const IBString &status, int filled,
		int remaining, double avgFillPrice, int permId, int parentId,
		double lastFillPrice, int clientId, const IBString& whyHeld) {}
	void staticDataRequestClient::historicalData(
		TickerId reqId, const IBString& date, double open, double high,
		double low, double close, int volume, int barCount, double WAP, int hasGaps) {}

}
