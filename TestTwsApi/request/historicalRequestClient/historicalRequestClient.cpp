/* Copyright (C) 2013 Interactive Brokers LLC. All rights reserved. This code is subject to the terms
* and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */

#include "request/historicalRequestClient/historicalRequestClient.hpp"

namespace IB {

	historicalRequestClient::historicalRequestClient(						// ctor
		const Contract & ct,
		const thOth::dateTime & dt,
		const barSize bar,
		const int length,
		const dataDuration dur,
		const dataType type) 
		
		: request       (ct    ),
		  endDate_      (dt    ),
		  length_       (length),
		  barSize_      (bar   ),
		  dataDuration_ (dur   ),
		  dataType_     (type  ) {}

	void historicalRequestClient::requestHistoricalData() {
	
		m_pClient->reqHistoricalData(										// call the corresponding EClientSocketBase method
			id_,															// request id
			contract_,														// contract
			convertDateTime(endDate_),										// date
			IB::utilities::dataDurationFactory()(dataDuration_, length_),	// whole day
			IB::utilities::barSizeFactory     ()(barSize_),					// 1 min bar
			IB::utilities::dataTypeFactory    ()(dataType_),				// dataType: only trades
			1,																// only data with regular trading hours
			1);																// date format: yyyymmdd{ space }{space}hh:mm : dd
	
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

		if (m_sleepDeadline > 0)											// initialize timeout with m_sleepDeadline - now
			tval.tv_sec = static_cast<long>(m_sleepDeadline - now);

		if (m_pClient->fd() >= 0) {

			FD_ZERO(&readSet);
			writeSet = readSet;

			FD_SET(m_pClient->fd(), &readSet);

			if (!m_pClient->isOutBufferEmpty())
				FD_SET(m_pClient->fd(), &writeSet);

			int ret = select(m_pClient->fd() + 1, &readSet, &writeSet, NULL, &tval);

			if (ret == 0)													// timeout
				return;

			if (ret < 0) {													// error
				disconnect();
				return;
			}

			if (FD_ISSET(m_pClient->fd(), &writeSet))				
				m_pClient->onSend();										// socket is ready for writing

			if (m_pClient->fd() < 0)
				return;

			if (FD_ISSET(m_pClient->fd(), &readSet))				
				m_pClient->onReceive();										// socket is ready for reading

		}
	}

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
		
		if (IsEndOfHistoricalData(date)) {									// control for EoF

			std::sort(bars_.begin(), bars_.end());							// finally sorting the bars
			notifyObservers();
			disconnect();
			return;

		}

		bars_.push_back(
			thOth::bar(
				convertDateTime(date),
				convertDateTime(date) + ToTimeDuration(barSize_),
				open,
				close,
				high,
				low,
				volume));

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
	void historicalRequestClient::orderStatus(
		OrderId orderId, const IBString &status, int filled,
		int remaining, double avgFillPrice, int permId, int parentId,
		double lastFillPrice, int clientId, const IBString& whyHeld) {}
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
