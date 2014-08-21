#include "functions/multiThreadedCsvBuilder/multiThreadedCsvBuilder.hpp"

void insertMatrix1(thOth::utilities::csvBuilder & csv) {								

	csv.add(boost::numeric::ublas::matrix<double>(100, 100, 1.0), 1, 1);
		
}

void insertMatrix2(thOth::utilities::csvBuilder & csv) {

	csv.add(boost::numeric::ublas::matrix<double>(100, 100, 2.0), 2, 2);
	
}

void multiThreadedCsvBuilder() {

	std::cout
		<< "multi-threaded csv builder test"
		<< std::endl
		<< "-------------------------------"
		<< std::endl;

	thOth::utilities::csvBuilder csv(CSVNAME);					// common csvBuilder

	std::thread thread1(insertMatrix1, std::ref(csv));
	std::thread thread2(insertMatrix2, std::ref(csv));

	thread1.join(); thread2.join();

	if (IB::settings::instance().verbosity() > 0)				// verbose

		TWS_LOG(std::string("end of multi-threaded csv builder test"))

};