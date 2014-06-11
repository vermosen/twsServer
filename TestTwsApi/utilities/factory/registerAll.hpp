#ifndef register_all
#define register_all

#include <utilities/factory/barSizeFactory/register_barSize.hpp>

// call all the register functions
namespace IB {

	namespace utilities {
	
		void registerAll() {

			registerBarSize();

		}
	
	}

}


#endif