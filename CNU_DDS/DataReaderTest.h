

#ifndef APPLICATION_DATAREADERTEST_H_
#define APPLICATION_DATAREADERTEST_H_

namespace CNU_DDS
{
	void	StreamingTest_R(unsigned long domain_id, unsigned long	num_of_endpoints, unsigned long topic_mode, unsigned long topic_entry, unsigned long num_of_data, unsigned long data_sending_delay);
	void	settingDefaultParametersForDataReader();
	void	dataReaderTest();
}
#endif /* APPLICATION_BEHAVIORTEST_H_ */