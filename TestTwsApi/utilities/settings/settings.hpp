/*
 *
 * custom TwsApi
 * Copyright (C) 2014 Jean-Mathieu Vermosen
 *
 */

#ifndef settings_hpp
#define settings_hpp

#include <string>
#include <mutex>

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

			inline idGenerator & generator () { return generator_; };
			inline logger      & log       () { return log_      ; };

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
			mutable std::mutex verbosityMutex_;		// locks/unlocks the verbosity variable
			mutable std::mutex portMutex_     ;		// locks/unlocks the port variable
			mutable std::mutex ibPortMutex_   ;		// locks/unlocks the ib port variable
			mutable std::mutex ibHostMutex_   ;		// locks/unlocks the ib host variable
			mutable std::mutex serverMutex_   ;		// locks/unlocks the server variable
			mutable std::mutex userMutex_     ;		// locks/unlocks the user variable
			mutable std::mutex passwordMutex_ ;		// locks/unlocks the password variable
			mutable std::mutex dataBaseMutex_;		// locks/unlocks the database variable
	};

}

#endif