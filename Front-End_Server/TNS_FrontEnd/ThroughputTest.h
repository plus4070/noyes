#pragma once

#include "DCPS/Domain/DomainParticipant.h"
#include "DCPS/Publication/Publisher.h"
#include "DCPS/Publication/DataWriter.h"
#include "DCPS/Subscription/Subscriber.h"
#include "DCPS/Subscription/DataReader.h"
#include "DCPS/Topic/Topic.h"
#include "DCPS/Domain/DomainParticipant.h"

#include "RTPS/Behavior/StatefulWriter.h"
#include "RTPS/Behavior/StatefulReader.h"

#include "Utility/PlatformOrientedInterface.h"
#include "Utility/SystemInformation.h"

#include "QoS/EntityQos/DomainParticipantQos.h"


class ThroughputTest
{
public:
	
	FILE*			file_pointer;
	char			chr_file_name[128];

	ThroughputTest(void);
	~ThroughputTest(void);
	
	void	StreamingTest();
	void	StreamingTest_Writer(unsigned long domain_id, unsigned long	num_of_endpoints, unsigned long topic_mode, unsigned long topic_entry, unsigned long num_of_data, unsigned long data_sending_delay);
	void	StreamingTest_Reader(unsigned long domain_id, unsigned long	num_of_endpoints, unsigned long topic_mode, unsigned long topic_entry, unsigned long num_of_data, unsigned long data_sending_delay);
};

