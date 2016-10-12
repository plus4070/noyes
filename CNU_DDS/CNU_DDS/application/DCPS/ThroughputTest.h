#ifndef APPLICATION_DCPS_THROUGHPUTTEST_H_
#define APPLICATION_DCPS_THROUGHPUTTEST_H_

namespace CNU_DDS
{
	void	ThroughputTest();
	void	StreamingTest();
	void	StreamingTest_Writer(unsigned long domain_id, unsigned long	num_of_endpoints, unsigned long topic_mode, unsigned long topic_entry, unsigned long num_of_data, unsigned long data_sending_delay);
	void	StreamingTest_Reader(unsigned long domain_id, unsigned long	num_of_endpoints, unsigned long topic_mode, unsigned long topic_entry, unsigned long num_of_data, unsigned long data_sending_delay);
}


#endif /* APPLICATION_DCPS_THROUGHPUTTEST_H_ */
