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
			void verbosity       (const unsigned int  v);
			void port            (const unsigned int  v);
			void ibPort          (const unsigned int  v);
			inline void ibHost   (const std::string & s) { ibHost_    = s; };
			inline void logPath  (const std::string & s) { log_.path(s)  ; };
			inline void server   (const std::string & s) { server_    = s; };
			inline void user     (const std::string & s) { user_      = s; };
			inline void password (const std::string & s) { password_  = s; };
			inline void dataBase (const std::string & s) { dataBase_  = s; };

			unsigned int verbosity      () const;
			unsigned int port           () const;
			unsigned int ibPort         () const;
			inline std::string ibHost   () const { return ibHost_   ; };
			inline std::string server   () const { return server_   ; };
			inline std::string user     () const { return user_     ; };
			inline std::string password () const { return password_ ; };
			inline std::string dataBase () const { return dataBase_ ; };

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


	};

}

#endif