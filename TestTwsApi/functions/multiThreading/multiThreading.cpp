#include "functions/multiThreading/multiThreading.hpp"

// this function associates an internal id 
// to a thread and returns a message.
void printCurrentId(std::string & res) {

	res.append("thread ");
	res.append(boost::lexical_cast<std::string>(std::this_thread::get_id()));
	res.append(" gets the internal id ");
	res.append(boost::lexical_cast<std::string>(IB::settings::instance().generator().next()));

}

// log in the thread
void logCurrentId() {

	std::string res;

	res.append("thread ");
	res.append(boost::lexical_cast<std::string>(
		std::this_thread::get_id()));
	res.append(" gets the internal id ");
	res.append(boost::lexical_cast<std::string>(
		IB::settings::instance().generator().next()));

	TWS_LOG(res)

}
void multiThreadingTest() {

	std::cout
		<< "multi-threading Test"
		<< std::endl
		<< "--------------------"
		<< std::endl;

	unsigned int const min_number = 2;							// min number of threads
	unsigned int const max_number = 20;							// max number of threads

	unsigned int threadNum =									// available hardware ressources
		std::thread::hardware_concurrency();

	threadNum =													// define number of threads
		min(max(threadNum, min_number), max_number);

	// test 1: log in the main thread
	std::vector<std::thread> threads1(threadNum);
	std::vector<std::string> results(threadNum);

	for (unsigned int i = 0; i < threadNum; i++)
		threads1[i] = std::thread(
			printCurrentId, 
			std::ref(
				results[i]));

	for (unsigned int i = 0; i < threadNum; i++)
		threads1[i].join();

	for (unsigned int i = 0; i < threadNum; i++) {

		std::cout << results[i] << std::endl;
		TWS_LOG(results[i])

	}

	// test 2: directly log in each thread
	std::vector<std::thread> threads2(threadNum);
	
	for (unsigned int i = 0; i < threadNum; i++)
		threads2[i] = std::thread(
			logCurrentId);

	for (unsigned int i = 0; i < threadNum; i++)
		threads2[i].join();

	if (IB::settings::instance().verbosity() > 0)				// verbose

		TWS_LOG(std::string("end of multi-threading test"))

};