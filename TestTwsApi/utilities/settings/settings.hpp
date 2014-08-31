/*
 *
 * custom TwsApi
 * Copyright (C) 2014 Jean-Mathieu Vermosen
 *
 */

#ifndef settings_hpp
#define settings_hpp

#include <string>

#include <boost/thread/mutex.hpp>
#include <boost/thread/lock_guard.hpp>

#include <thOth/pattern/singleton.hpp>

#include <commonDefs.h>

#include "utilities/idGenerator/idGenerator.hpp"
#include "utilities/logger/logger.hpp"

#define DEFAULTPATH "C:/Temp/"

namespace IB {

	// concurrency compatible setting class
	class settings : public thOth::singleton < settings > {
												
		friend class thOth::singleton < settings >;

		private:

			settings()						// default values
				: verbosity_(0),
				  port_(0),
				  log_(DEFAULTPATH) {};

		public: 
			
			//accessors
			void verbosity (const unsigned int  v);
			void port      (const unsigned int  v);
			void ibPort    (const unsigned int  v);
			void ibHost    (const std::string & s);
			void server    (const std::string & s);
			void user      (const std::string & s);
			void password  (const std::string & s);
			void dataBase  (const std::string & s);

			inline void logPath(const std::string & s) { log_.path(s); };

			unsigned int verbosity () const;
			unsigned int port      () const;
			unsigned int ibPort    () const;
			std::string ibHost     () const;
			std::string server     () const;
			std::string user       () const;
			std::string password   () const;
			std::string dataBase   () const;

			inline idGenerator & idGen () { return generator_; };
			inline logger      & log   () { return log_      ; };

		private: 

			// members
			unsigned int verbosity_;				// verbosity settings				
			idGenerator  generator_;				// unique id generator
			logger             log_;				// log file

			// dataBase settings
			unsigned int port_     ;     			// db server port
			std::string  server_   ;				// db server name
			std::string  user_     ;				// user name
			std::string  password_ ;				// user password
			std::string  dataBase_ ;				// dataBase name

			// interactive broker settings
			std::string  ibHost_   ;				// interactive broker server ip
			unsigned int ibPort_   ;				// interactive broker server port

			// mutexes
			mutable boost::mutex verbosityMutex_;	// locks/unlocks the verbosity variable
			mutable boost::mutex portMutex_;		// locks/unlocks the port variable
			mutable boost::mutex ibPortMutex_;		// locks/unlocks the ib port variable
			mutable boost::mutex ibHostMutex_;		// locks/unlocks the ib host variable
			mutable boost::mutex serverMutex_;		// locks/unlocks the server variable
			mutable boost::mutex userMutex_;		// locks/unlocks the user variable
			mutable boost::mutex passwordMutex_;	// locks/unlocks the password variable
			mutable boost::mutex dataBaseMutex_;	// locks/unlocks the database variable
	};

}

#endif