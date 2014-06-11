/* Copyright (C) 2013 Interactive Brokers LLC. All rights reserved. This code is subject to the terms
* and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */

#include "historicalRequestClient.hpp"
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

	const int PING_DEADLINE = 2; // seconds
	const int SLEEP_BETWEEN_PINGS = 30; // seconds

	///////////////////////////////////////////////////////////
	// member funcs
	historicalRequestClient::historicalRequestClient(
		const Contract & ct,
		const thOth::dateTime & dt)

		: m_pClient(new EPosixClientSocket(this)),
		  m_state(ST_CONNECT),
		  m_sleepDeadline(0),
		  endDate_(dt) {}

	historicalRequestClient::~historicalRequestClient() {}

	historicalRequestClient & historicalRequestClient::operator = (const historicalRequestClient & o) {
	
		// member copy
		if (this != &o) {
		
			ts_                  = o.ts_                 ;
			endOfHistoricalData_ = o.endOfHistoricalData_;
			errorForRequest_     = o.errorForRequest_    ;
			marketDataType_      = o.marketDataType_     ;
			m_pClient            = o.m_pClient           ;
			m_state              = o.m_state             ;
			m_sleepDeadline      = o.m_sleepDeadline     ;
		
		}
	
		return *this;
	
	};

	bool historicalRequestClient::connect(const char *host, unsigned int port, int clientId) {

		// trying to connect
		bool bRes = m_pClient->eConnect2(host, port, clientId);
		return bRes;

	}

	void historicalRequestClient::disconnect() const {

		m_pClient->eDisconnect();

	}

	bool historicalRequestClient::isConnected() const {

		return m_pClient->isConnected();

	}

	void historicalRequestClient::requestHistoricalData() {
	
		// generates an id -> guid generator ?
		TickerId id = 12;

		// call the corresponding EClientSocketBase method
		m_pClient->reqHistoricalData(
			id,											  // request id
			contract_,									  // contract
			convertDateTime(endDate_),					  // date
			IB::utilities::dataDurationFactory()(day, 1), // whole day
			IB::utilities::barSizeFactory     ()(oneDay), // 1 min bar
			IB::utilities::dataTypeFactory    ()(trades), // dataType: only trades
			1,											  // only data with regular trading hours
			1);											  // date format: yyyymmdd{ space }{space}hh:mm : dd
	
		m_state = ST_REQUEST_ACK;
	}

	void historicalRequestClient::processMessages() {

		fd_set readSet, writeSet;

		struct timeval tval;
		tval.tv_usec = 0;
		tval.tv_sec = 0;

		time_t now = time(NULL);

		switch (m_state) {

		case ST_REQUEST:
			requestHistoricalData();
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

		if (m_sleepDeadline > 0)
			tval.tv_sec = m_sleepDeadline - now;			// initialize timeout with m_sleepDeadline - now

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
	void historicalRequestClient::reqCurrentTime()
	{

		m_sleepDeadline = time(NULL) + PING_DEADLINE;		// set ping deadline to "now + n seconds"
		m_state = ST_PING_ACK;
		m_pClient->reqCurrentTime();

	}

	///////////////////////////////////////////////////////////////////
	// events
	void historicalRequestClient::orderStatus(OrderId orderId, const IBString &status, int filled,
		int remaining, double avgFillPrice, int permId, int parentId,
		double lastFillPrice, int clientId, const IBString& whyHeld) {}

	void historicalRequestClient::nextValidId(OrderId orderId) {
	
		m_state = ST_REQUEST;
	
	}

	void historicalRequestClient::currentTime(long time) {

		if (m_state == ST_PING_ACK) {
			
			time_t t = (time_t)time;
			struct tm * timeinfo = localtime(&t);
			time_t now = ::time(NULL);
			m_sleepDeadline = now + SLEEP_BETWEEN_PINGS;
			m_state = ST_IDLE;

		}
	}

	void historicalRequestClient::error(const int id, const int errorCode, const IBString errorString) {

		if (id == -1 && errorCode == 1100)					// if "Connectivity between IB and TWS has been lost"
			disconnect();
		else
			std::cout
				<< "error has occured: "
				<< std::endl
				<< errorString;

	}



	thOth::dateTime historicalRequestClient::convertDateTime(const IBString & dtStr) const {

		return thOth::dateTime(								// smallest increments is second
			thOth::dateTime::Years(boost::lexical_cast<int>(dtStr.substr(0, 4))),
			thOth::dateTime::Months(boost::lexical_cast<int>(dtStr.substr(4, 2))),
			thOth::dateTime::Days(boost::lexical_cast<int>(dtStr.substr(6, 2))),
			thOth::dateTime::Hours(boost::lexical_cast<int>(dtStr.substr(10, 2))),
			thOth::dateTime::Minutes(boost::lexical_cast<int>(dtStr.substr(13, 2))),
			thOth::dateTime::Seconds(boost::lexical_cast<int>(dtStr.substr(16, 2))));

	};

	IBString historicalRequestClient::convertDateTime(const thOth::dateTime & date) const {
	
		std::stringstream stream;							
		boost::posix_time::time_facet* facet = new boost::posix_time::time_facet();
		facet->format("%Y%m%d  %H:%M:%S");
		stream.imbue(std::locale(std::locale::classic(), facet));
		stream << date;
		
		IBString ib(stream.str());

		return ib;

	};

	void historicalRequestClient::historicalData(
		TickerId reqId, 
		const IBString& date, 
		double open, 
		double high, 
		double low, 
		double close, 
		int volume, 
		int barCount, 
		double WAP, 
		int hasGaps) {

		// control for EoF
		//if (m_pClient->fd()) {

		//	endOfHistoricalData_ = true;
		//	return;

		//}

		ts_.insert(std::pair<thOth::dateTime, IB::historicalQuoteDetails>(			// copy the current date in the container
			this->convertDateTime(date),
			IB::historicalQuoteDetails{ 
				reqId, 
				open, 
				high,
				low, 
				close, 
				volume,
				barCount, 
				WAP, 
				hasGaps }));

	}

	void historicalRequestClient::tickPrice(TickerId tickerId, TickType field, double price, int canAutoExecute) {}
	void historicalRequestClient::tickSize(TickerId tickerId, TickType field, int size) {}
	void historicalRequestClient::tickOptionComputation(TickerId tickerId, TickType tickType, double impliedVol, double delta,
		double optPrice, double pvDividend,
		double gamma, double vega, double theta, double undPrice) {}
	void historicalRequestClient::tickGeneric(TickerId tickerId, TickType tickType, double value) {}
	void historicalRequestClient::tickString(TickerId tickerId, TickType tickType, const IBString& value) {}
	void historicalRequestClient::tickEFP(TickerId tickerId, TickType tickType, double basisPoints, const IBString& formattedBasisPoints,
		double totalDividends, int holdDays, const IBString& futureExpiry, double dividendImpact, double dividendsToExpiry) {}
	void historicalRequestClient::openOrder(OrderId orderId, const Contract&, const Order&, const OrderState& ostate) {}
	void historicalRequestClient::openOrderEnd() {}
	void historicalRequestClient::winError(const IBString &str, int lastError) {}
	void historicalRequestClient::connectionClosed() {}
	void historicalRequestClient::updateAccountValue(const IBString& key, const IBString& val,
		const IBString& currency, const IBString& accountName) {}
	void historicalRequestClient::updatePortfolio(const Contract& contract, int position,
		double marketPrice, double marketValue, double averageCost,
		double unrealizedPNL, double realizedPNL, const IBString& accountName){}
	void historicalRequestClient::updateAccountTime(const IBString& timeStamp) {}
	void historicalRequestClient::accountDownloadEnd(const IBString& accountName) {}
	void historicalRequestClient::contractDetails(int reqId, const ContractDetails& contractDetails) {}
	void historicalRequestClient::bondContractDetails(int reqId, const ContractDetails& contractDetails) {}
	void historicalRequestClient::contractDetailsEnd(int reqId) {}
	void historicalRequestClient::execDetails(int reqId, const Contract& contract, const Execution& execution) {}
	void historicalRequestClient::execDetailsEnd(int reqId) {}

	void historicalRequestClient::updateMktDepth(TickerId id, int position, int operation, int side,
		double price, int size) {}
	void historicalRequestClient::updateMktDepthL2(TickerId id, int position, IBString marketMaker, int operation,
		int side, double price, int size) {}
	void historicalRequestClient::updateNewsBulletin(int msgId, int msgType, const IBString& newsMessage, const IBString& originExch) {}
	void historicalRequestClient::managedAccounts(const IBString& accountsList) {}
	void historicalRequestClient::receiveFA(faDataType pFaDataType, const IBString& cxml) {}
	void historicalRequestClient::scannerParameters(const IBString &xml) {}
	void historicalRequestClient::scannerData(int reqId, int rank, const ContractDetails &contractDetails,
		const IBString &distance, const IBString &benchmark, const IBString &projection,
		const IBString &legsStr) {}
	void historicalRequestClient::scannerDataEnd(int reqId) {}
	void historicalRequestClient::realtimeBar(TickerId reqId, long time, double open, double high, double low, double close,
		long volume, double wap, int count) {}
	void historicalRequestClient::fundamentalData(TickerId reqId, const IBString& data) {}
	void historicalRequestClient::deltaNeutralValidation(int reqId, const UnderComp& underComp) {}
	void historicalRequestClient::tickSnapshotEnd(int reqId) {}
	void historicalRequestClient::marketDataType(TickerId reqId, int marketDataType) {}
	void historicalRequestClient::commissionReport(const CommissionReport& commissionReport) {}
	void historicalRequestClient::position(const IBString& account, const Contract& contract, int position, double avgCost) {}
	void historicalRequestClient::positionEnd() {}
	void historicalRequestClient::accountSummary(int reqId, const IBString& account, const IBString& tag, const IBString& value, const IBString& curency) {}
	void historicalRequestClient::accountSummaryEnd(int reqId) {}

}
