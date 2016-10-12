#include "TestListener.h"
#include "../../include/Utility/SystemDefinition.h"
#include "../../include/DCPS/Subscription/DataReader.h"
#include "../../include/RTPS/Structure/Reader.h"
#include <stdio.h>

namespace CNU_DDS
{
	TestListener::TestListener()
	{
		count	= 0;
	}

	TestListener::~TestListener()
	{

	}

	void	TestListener::on_data_available(pDataReader the_reader)
	{
		//printf("TestListener::on_data_available()\n");
		if(count == 0)
		{
			time_start	= get_current_time();
			printf("Start(%d)\n", time_start);
		}
		count++;

		if(the_reader)
		{
			HistoryCache*	hc	= the_reader->related_rtps_reader->reader_cache;
			pCacheChange	cc	= NULL;

			while(hc->changes->getSize())
			{
				cc	= hc->changes->popFirst();
				//printf("SeqNum(%d, %d)\n", cc->sequence_number.high, cc->sequence_number.low);
			}
		}

		if(count >= limit)
		{
			time_end	= get_current_time();
			printf("End(%d)\n", time_end);
			printf("Receiving Time : %d(ms)\n", time_end - time_start);
			exit(0);
		}
	}

	void	TestListener::on_sample_rejected(pDataReader the_reader, SampleRejectedStatus* status)
	{
		printf("TestListener::on_sample_rejected()\n");
	}

	void	TestListener::on_liveliness_changed(pDataReader the_reader, LivelinessChangedStatus* status)
	{
		printf("TestListener::on_liveliness_changed()\n");
	}

	void	TestListener::on_requested_deadline_missed(pDataReader the_reader, RequestedDeadlineMissedStatus* status)
	{
		printf("TestListener::on_requested_deadline_missed()\n");
	}

	void	TestListener::on_requested_incompatible_qos(pDataReader the_reader, RequestedIncompatibleQosStatus* status)
	{
		printf("TestListener::on_requested_incompatible_qos()\n");
	}

	void	TestListener::on_subscription_matched(pDataReader the_reader, SubscriptionMatchedStatus* status)
	{
		printf("TestListener::on_subscription_matched()\n");
	}

	void	TestListener::on_sample_lost(pDataReader the_reader, SampleLostStatus* status)
	{
		printf("TestListener::on_sample_lost()\n");
	}
}
