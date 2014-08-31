/* Copyright (C) 2013 Interactive Brokers LLC. All rights reserved. This code is subject to the terms
* and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */

#include "request/request.hpp"

namespace IB {

	request::request(const Contract & ct)									// ctor

		: m_pClient(new EPosixClientSocket(this)),
		  m_state(ST_CONNECT),
		  m_sleepDeadline(0),
		  contract_(ct) {
	
		requestId();														// generate the id
	
	};

	bool request::connect(
		const char *host,
		unsigned int port) {

		return m_pClient->eConnect2(											// trying to connect
			host, port, id_);

	}

	void request::reqCurrentTime() {

		m_sleepDeadline = time(NULL) + PING_DEADLINE;						// set ping deadline to "now + n seconds"
		m_state = ST_PING_ACK;
		m_pClient->reqCurrentTime();

	}

	bool request::IsEndOfData(const IBString& Date) {						// check if static request has been achieve

		endOfData_ = 1 + strncmp((const char*)Date.data(), "finished", 8);	// todo: check for request achivement
		return endOfData_;

	}

	void request::nextValidId(OrderId orderId) {

		m_state = ST_REQUEST;

	}

	void request::currentTime(long time) {

		if (m_state == ST_PING_ACK) {

			time_t t = (time_t)time;
			struct tm * timeinfo = localtime(&t);
			time_t now = ::time(NULL);
			m_sleepDeadline = now + SLEEP_BETWEEN_PINGS;
			m_state = ST_IDLE;

		}
	}

	void request::error(													// error management
		const int id,														// todo: create exceptions and throw
		const int errorCode,
		const IBString errorString) {

		if (errorCode == 1100) {											// if "Connectivity between IB and TWS has been lost"

			TWS_LOG_V(
				std::string("request error: ")								// log
				.append("request object with id ")
				.append(boost::lexical_cast<std::string>(id_))
				.append("lost the connection with IB server"), 0)

				disconnect();

		} else if (errorCode == 200) {										// contract has no match

			TWS_LOG_V(
				std::string("request error: ")								// log
				.append("request object with id ")
				.append(boost::lexical_cast<std::string>(id_))
				.append("contract has no match"), 0)

				disconnect();

		} else {

			TWS_LOG_V(
				std::string("request information: ")						// log
				.append("request object with id ")
				.append(boost::lexical_cast<std::string>(id_))
				.append("generated the following message: ")
				.append(errorString), 0)

		}
	}

	void request::contractDetailsEnd(int reqId) {

		// no idea of it's usage
		std::cout << "contractDetailsEnd has been called" << std::endl;

	}

}
