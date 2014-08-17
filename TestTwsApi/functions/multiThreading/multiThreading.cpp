#include "functions/multiThreading/multiThreading.hpp"

// function used to 
void printCurrentId(std::string & res) {

	std::thread::id id = std::this_thread::get_id();			// get current thread id

	long nextId = IB::settings::instance().generator().next();	// next id

	res.append("thread ");
	res.append(boost::lexical_cast<std::string>(std::this_thread::get_id()));
	res.append(" get the internal id ");
	res.append(boost::lexical_cast<std::string>(IB::settings::instance().generator().next()));

}
void multiThreadingTest() {

	std::cout
		<< "multi-threading Test"
		<< std::endl
		<< "--------------------"
		<< std::endl;

	std::vector<std::thread> threads; 
	std::vector<std::string> results;

	for (int i = 0; i < 20; i++) {
	
		results.push_back(std::string());
		threads.push_back(std::thread(printCurrentId, results.back()));

	}

	for (int i = 0; i < 20; i++)
		threads[i].join();

	for (int i = 0; i < 20; i++) {

		std::cout << results[i] << std::endl;
		TWS_LOG(results[i])

	}

	
	if (IB::settings::instance().verbosity() > 0)				// verbose

		TWS_LOG(std::string("end of multi-threading test"))

};