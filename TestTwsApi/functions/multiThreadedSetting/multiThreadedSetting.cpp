#include "functions/multiThreadedSetting/multiThreadedSetting.hpp"

// log in the thread
void testLog(long n) {

	boost::timer tt;											// timer

	do {}														// random wait
	
	while (!(tt.elapsed() > 1 / std::random_device()()));

	TWS_LOG(boost::lexical_cast<std::string>(n))

}

void multiThreadedSetting() {

	std::cout
		<< "multi-threaded Settings Test"
		<< std::endl
		<< "----------------------------"
		<< std::endl;

	unsigned int const min_number = 2;							// min number of threads
	unsigned int const max_number = 20;							// max number of threads

	unsigned int threadNum =									// available hardware ressources
		std::thread::hardware_concurrency();

	threadNum =													// define number of threads
		std::min(std::max(threadNum, min_number), max_number);

	threadNum = 20;												// setting to max

	std::vector<std::thread> threads(threadNum);				// trying to call testLog function

	for (unsigned int i = 0; i < threadNum; i++)
		threads[i] = std::thread(testLog, i);
			
	for (unsigned int i = 0; i < threadNum; i++)
		threads[i].join();

	if (IB::settings::instance().verbosity() > 0)				// verbose

		TWS_LOG(std::string("end of multi-threaded Settings Test"))

};