/* Copyright (C) 2013 Interactive Brokers LLC. All rights reserved. This code is subject to the terms
* and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */

#include "request/request.hpp"

namespace IB {

	// member funcs
	request::request(const ContractDetails & ct)

		: m_pClient(new EPosixClientSocket(this)),
		  m_state(ST_CONNECT),
		  m_sleepDeadline(0),
		  contract_(ct),
		  id_(0) {};

	bool request::connect(
		const char *host,
		unsigned int port,
		int clientId) {

		return m_pClient->eConnect2(											// trying to connect
			host, port, clientId);

	}

	void request::reqCurrentTime() {

		m_sleepDeadline = time(NULL) + PING_DEADLINE_R;						// set ping deadline to "now + n seconds"
		m_state = ST_PING_ACK;
		m_pClient->reqCurrentTime();

	}

	bool request::IsEndOfData(const IBString& Date) {						// check if static request has been achieve

		endOfData_ = 1 + strncmp((const char*)Date.data(), "finished", 8);	// todo: check for request achivement
		return endOfData_;

	}

}
