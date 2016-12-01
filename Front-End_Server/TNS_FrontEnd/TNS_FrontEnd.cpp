// TNS_FrontEnd.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//
#include "stdafx.h"
#include "ThroughputTest.h"
#include "TNSNServer.h"

#include "DCPS/Domain/DomainParticipant.h"
#include "RTPS/Discovery/SEDPdiscoveredPublicationData.h"
#include "RTPS/Discovery/SEDPdiscoveredSubscriptionData.h"
#include "Utility/Monitoring/Monitor.h"

#include "DCPS/Domain/DomainParticipant.h"
#include "DCPS/Publication/Publisher.h"
#include "DCPS/Publication/DataWriter.h"
#include "DCPS/Subscription/Subscriber.h"
#include "DCPS/Subscription/DataReader.h"
#include "DCPS/Topic/Topic.h"



int _tmain(int argc, _TCHAR* argv[])
{
	using namespace CNU_DDS;

	Monitor*	mon	= Monitor::getMonitorInstance();
	start_local_time();

	int tnsOption = 0, testOption = 0;

	unsigned long	domain_id = 10;
	unsigned long	endpoint_type = 45;
	unsigned long	num_of_endpoints = 45;
	unsigned long	topic_mode = 2;
	unsigned long	topic_number = 1;
	unsigned long	num_of_data = 250;
	unsigned long	data_sending_delay = 800;
	
	printf("[ 1 : Start Front-End Server / 2 : ThroughputTest Start\n");
	printf(" input > ");
	scanf("%d", &tnsOption);

	StartTopicNamingService(tnsOption);

	if (tnsOption == 2) {
		ThroughputTest *Test = new ThroughputTest();

		printf("1 : DataWriter(1-45)\n2 : DataReader(1-45)\n3 : DataWriter(46-90)\n4 : DataReader(46-90)\n");
		printf(" input > ");
		scanf("%d", &testOption);

		switch (testOption) {
		case 1:
			Test->StreamingTest_Writer(domain_id, num_of_endpoints, topic_mode, topic_number, num_of_data, data_sending_delay);
			break;
		case 2:
			Test->StreamingTest_Reader(domain_id, num_of_endpoints, topic_mode, topic_number, num_of_data, data_sending_delay);
			break;
		case 3:
			topic_number = 46;
			Test->StreamingTest_Writer(domain_id, num_of_endpoints, topic_mode, topic_number, num_of_data, data_sending_delay);
			break;
		case 4:
			topic_number = 46;
			Test->StreamingTest_Reader(domain_id, num_of_endpoints, topic_mode, topic_number, num_of_data, data_sending_delay);
			break;
		case 5:
			topic_number = 51;
			num_of_endpoints = 10;
			Test->StreamingTest_Reader(domain_id, num_of_endpoints, topic_mode, topic_number, num_of_data, data_sending_delay);
		default:
			break;
		}
	}

	scanf("%d", &testOption);
}

