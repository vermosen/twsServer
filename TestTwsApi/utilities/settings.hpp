#ifndef settings_hpp
#define settings_hpp

#include <string>

#include <thOth/pattern/singleton.hpp>

namespace IB {

	class settings : public thOth::singleton < settings > {

		friend class thOth::singleton < settings > ;

	private:

		settings()							// default values
			: verbosity_(0),
			  port_     (0) {}; 

		public: 
			
			//accessors
			inline void verbosity(const int           v) { verbosity_ = v; };
			inline void port     (const int           v) { port_      = v; };
			inline void ibPort   (const int           v) { ibPort_    = v; };
			inline void ibHost   (const std::string & s) { ibHost_    = s; };
			inline void logPath(const std::string & s) { logPath_     = s; };
			inline void server   (const std::string & s) { server_    = s; };
			inline void user     (const std::string & s) { user_      = s; };
			inline void password (const std::string & s) { password_  = s; };
			inline void dataBase (const std::string & s) { dataBase_  = s; };

			inline int         verbosity() const { return verbosity_; };
			inline int         port     () const { return port_     ; };
			inline int         ibPort   () const { return ibPort_   ; };
			inline std::string ibHost   () const { return ibHost_   ; };
			inline std::string logPath  () const { return server_   ; };
			inline std::string server   () const { return server_   ; };
			inline std::string user     () const { return user_     ; };
			inline std::string password () const { return password_ ; };
			inline std::string dataBase () const { return dataBase_ ; };

		private: 

			// debug settings
			int         verbosity_;     	// verbosity settings				
			std::string logPath_  ;			// log file path

			// dataBase settings
			int         port_     ;     	// server port
			std::string server_   ;			// db server name
			std::string user_     ;			// user name
			std::string password_ ;			// user password
			std::string dataBase_ ;			// dataBase name

			// interactive broker settings
			std::string ibHost_   ;			// interactive broker ip
			int         ibPort_   ;			// interactive broker port
	
	};

}

#endif