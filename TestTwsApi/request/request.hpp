/*
*
* custom TwsApi
* Copyright (C) 2014 Jean-Mathieu Vermosen
*
*/

#ifndef request_hpp
#define request_hpp

#include <stdio.h>																		// printf()
#include <time.h>

#include <boost/shared_ptr.hpp>

#include <thOth/time/DateTime.hpp>
#include <thOth/time/timeseries.hpp>
#include <thOth/pattern/observable.hpp>

#include "EPosixClientSocket.h"															// tws components
#include "Ewrapper.h"
#include "EPosixClientSocketPlatform.h"
#include "Contract.h"

#include "utilities/conversion/convertDateTime/convertDateTime.hpp"
#include "utilities/settings/settings.hpp"

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

	const int PING_DEADLINE_R = 2;														// seconds
	const int SLEEP_BETWEEN_PINGS_R = 30;												// seconds

	class EPosixClientSocket;
	struct Contract;

	// base class for any concrete Ewrapper implementation
	class request : public EWrapper, thOth::observable {

	private:

		enum state {																	// client states

			ST_CONNECT,
			ST_REQUEST,
			ST_REQUEST_ACK,
			ST_PING,
			ST_PING_ACK,
			ST_IDLE

		};

	private:

		request              (               ) = delete;								// no default ctor
		request              (const request &) = delete;								// no copy ctor
		request & operator = (const request &) = delete;								// no assignement operator

		inline void requestId() { id_ = IB::settings::instance().generator().next(); };	// request a new interactive broker id
		void reqCurrentTime();															// get the current time (TODO: use stl)

	public:

		request(const ContractDetails &);												// provides the contract
		~request() {};																	// destructor

		// accessors
		bool endOfData      () const { return endOfData_      ; };						// end of data
		bool errorForRequest() const { return errorForRequest_; };						// error, TODO: throw exception

		Contract        contract       () const { return contract_.summary; };			// read access for contract
		ContractDetails contractDetails() const { return contract_ ; };					// read access for contract details

		void processMessages();

		bool connect(
			const char * host,
			unsigned int port,
			int clientId = 0);

		inline void disconnect () const { m_pClient->eDisconnect       (); };
		inline bool isConnected() const { return m_pClient->isConnected(); };

	protected:

		// maybe useless
		bool IsEndOfData(const IBString& Date);

	private:

		bool endOfData_  ;															// indicate whether the file has been read
		bool errorForRequest_;														// error on the request
		int marketDataType_  ;														// market data type

		ContractDetails contract_;													// the initial contract details
		TickerId id_;																// id of the request

		boost::shared_ptr<EPosixClientSocket> m_pClient;							// posix client. Turn into a static member ?

		state m_state;																// current state
		time_t m_sleepDeadline;														// sleep deadline

	};

}
#endif