#include "ThroughputListener.h"
#include "../../include/DCPS/Subscription/DataReader.h"
#include "../../include/RTPS/Structure/Reader.h"
#include "../../include/RTPS/Behavior/StatefulReader.h"
#include "../../include/Utility/Network/Transceiver.h"
#include <stdio.h>

namespace CNU_DDS
{
	unsigned long	ThroughputListener::listener_count;
	unsigned long	ThroughputListener::over_count;

	ThroughputListener::ThroughputListener()
	{
		limit	= 0;
		count	= 0;
	}

	ThroughputListener::~ThroughputListener()
	{
		printf("\nThroughputListener::~ThroughputListener() count==%d\n", count);
	}

	void	ThroughputListener::on_data_available(pDataReader the_reader)
	{

		HistoryCache*	hc	= the_reader->related_rtps_reader->reader_cache;
		pCacheChange	cc	= NULL;

		if(count == 0)
		{
			time_start	= get_current_time();
		}

		//count++;
		//count				= hc->changes->getSize();

		if(the_reader)
		{
			if(hc->changes->getSize())
			{
				//cc	= hc->changes->popFirst();
				count++;
				//cc	= hc->changes->popFirst();
				//printf("SeqNum(%d, %d)\n", cc->sequence_number.high, cc->sequence_number.low);
			}
		}

		if(count >= limit)
		{
			ThroughputListener::over_count++;

			time_end	= get_current_time();

			fprintf(fp, "%d,%d,%d\n", time_start, time_end, time_end-time_start);
			fflush(fp);
/*
			unsigned long	proc_count	= ((Transceiver*)((StatefulReader*)the_reader->related_rtps_reader)->_get_transceiver())->test_proc_count;
			unsigned long	recv_count	= ((Transceiver*)((StatefulReader*)the_reader->related_rtps_reader)->_get_transceiver())->test_recv_count;
			printf("\nTransceiver::proc_count(%d), Transceiver::recv_count(%d)\n", proc_count, recv_count);
*/
			if(ThroughputListener::over_count >= ThroughputListener::listener_count)
			{
				printf("\n*** Throughput test is over ***\n");
				fclose(fp);
				exit(0);
			}
		}
	}

	void	ThroughputListener::on_sample_rejected(pDataReader the_reader, SampleRejectedStatus* status)
	{

	}

	void	ThroughputListener::on_liveliness_changed(pDataReader the_reader, LivelinessChangedStatus* status)
	{

	}

	void	ThroughputListener::on_requested_deadline_missed(pDataReader the_reader, RequestedDeadlineMissedStatus* status)
	{

	}

	void	ThroughputListener::on_requested_incompatible_qos(pDataReader the_reader, RequestedIncompatibleQosStatus* status)
	{

	}

	void	ThroughputListener::on_subscription_matched(pDataReader the_reader, SubscriptionMatchedStatus* status)
	{

	}

	void	ThroughputListener::on_sample_lost(pDataReader the_reader, SampleLostStatus* status)
	{

	}
}
