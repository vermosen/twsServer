#ifndef settings_hpp
#define settings_hpp

#include <thOth/pattern/singleton.hpp>

namespace IB {

	class settings : public thOth::singleton < settings > {

		friend class thOth::singleton < settings > ;

	private:

		settings() : verbosity_(0) {}; // default values

		public: 
			
			//accessors
			inline void verbosity(const int v) { verbosity_ = v; };
			inline int verbosity() const { return verbosity_; };

		private: 

			// variables
			int verbosity_;

	};

}

#endif