#include "functions/multiThreadedSetting/multiThreadedSetting.hpp"

void acquireTickerId(std::string & result) {					// returns a message

	boost::timer tt;											// timer
	
	float wait = 1.0 / static_cast<float>(std::random_device()());

	while (tt.elapsed() < std::min(wait, float(.1)));					// wait some time
	
	result
		.append("thread number ")
		.append(boost::lexical_cast<std::string>(
			std::this_thread::get_id()))
		.append(" get the id ")
		.append(boost::lexical_cast<std::string>(
			IB::settings::instance().generator().next()));

	if (IB::settings::instance().verbosity() > 1)				// verbose ?
		TWS_LOG(result)											// log

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

	threadNum = 20;												// enforce number of threads to max

	std::vector<std::thread> threads (threadNum);				// trying to call testLog function
	std::vector<std::string> messages(threadNum);				// the messages

	for (unsigned int i = 0; i < threadNum; i++)				// launches a new thread
		threads[i] = std::thread(acquireTickerId, std::ref(messages[i]));
			
	for (unsigned int i = 0; i < threadNum; i++)				// join
		threads[i].join();

	for (unsigned int i = 0; i < threadNum; i++)				// read the messages

		std::cout 
			<< messages[i]
			<< std::endl;

	if (IB::settings::instance().verbosity() > 0)				// verbose
		TWS_LOG(std::string("end of multi-threaded Settings Test"))

};