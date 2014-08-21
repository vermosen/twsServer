#include "functions/multiThreadedCsvBuilder/multiThreadedCsvBuilder.hpp"

// log in the thread
void testCsv(long n) {

	boost::timer tt;

	// wait
	do {} while (!(tt.elapsed() > 1 / std::random_device()()));

	TWS_LOG(boost::lexical_cast<std::string>(n))

}

void multiThreadedCsvBuilder() {

	std::cout
		<< "multi-threaded csv builder Test"
		<< std::endl
		<< "-------------------------------"
		<< std::endl;

	unsigned int const min_number = 2;							// min number of threads
	unsigned int const max_number = 20;							// max number of threads

	unsigned int threadNum =									// available hardware ressources
		std::thread::hardware_concurrency();

	threadNum =													// defines accurate number of threads
		std::min(std::max(threadNum, min_number), max_number);

	std::vector<std::thread> threads(threadNum);				// trying to call testLog function

	for (unsigned int i = 0; i < threadNum; i++)
		threads[i] = std::thread(testCsv, i);
			
	for (unsigned int i = 0; i < threadNum; i++)
		threads[i].join();

	if (IB::settings::instance().verbosity() > 0)				// verbose

		TWS_LOG(std::string("end of multi-threading test"))

};