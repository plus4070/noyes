#include "../../../include/DCPS/BuiltinEntity/BuiltinSubscriptionsReaderListener.h"
#include "../../../include/DCPS/Domain/DomainParticipant.h"
#include "../../../include/DCPS/Subscription/DataReader.h"

#include "../../../include/RTPS/Discovery/SEDPdiscoveredSubscriptionData.h"
#include "../../../include/RTPS/Structure/Participant.h"
#include "../../../include/RTPS/Structure/CacheChange.h"
#include "../../../include/RTPS/Behavior/StatefulReader.h"

#include "../../../include/Utility/Monitoring/Monitor.h"

#include <stdio.h>

namespace CNU_DDS
{
	BuiltinSubscriptionsReaderListener::BuiltinSubscriptionsReaderListener()
	{

	}

	BuiltinSubscriptionsReaderListener::~BuiltinSubscriptionsReaderListener()
	{

	}

	void	BuiltinSubscriptionsReaderListener::on_data_available(pDataReader the_reader)
	{
		StatefulReader*					rtps_reader		= (StatefulReader*)the_reader->related_rtps_reader;
		Participant*					rtps_part		= the_reader->related_participant->related_rtps_participant;
		ParticipantProxy*				recv_part		= NULL;
		SEDPdiscoveredSubscriptionData*	recv_edp		= NULL;
		pCacheChange					change			= NULL;
		Locator_t*						locator;

		Monitor*						monitor			= Monitor::getMonitorInstance();
		Locator_t*						monitoring_locator	= NULL;

		//printf("Sub_ReaderListener::on_data_available()\n");

		while(rtps_reader->reader_cache->changes->getSize() > 0)
		{
			recv_edp	= new SEDPdiscoveredSubscriptionData();
			change		= rtps_reader->reader_cache->changes->popFirst();

			recv_edp->Deserialize(change->data_value);

			recv_part	= rtps_part->get_remote_participant(recv_edp->remote_reader_guid.guid_prefix);

			if(recv_part)
			{
				/* Insert a default_unicast_locator from recv_participant to recv_edp */
				/* and others? */
				locator	= new Locator_t;
				memcpy(locator, recv_part->default_unicast_locator_list->getFirst(), sizeof(Locator_t));
				recv_edp->unicast_locator_list->insertInRear(locator);
				the_reader->related_participant->_remote_datareader_is_discovered(recv_edp);
			}

			if(monitor)
			{
				monitoring_locator	= new Locator_t;
				memcpy(monitoring_locator, locator, sizeof(Locator_t));
				monitor->insertReaderData(change->data_value, monitoring_locator);
			}
		}
	}

	void	BuiltinSubscriptionsReaderListener::on_sample_rejected(pDataReader the_reader, SampleRejectedStatus* status)
	{

	}

	void	BuiltinSubscriptionsReaderListener::on_liveliness_changed(pDataReader the_reader, LivelinessChangedStatus* status)
	{

	}

	void	BuiltinSubscriptionsReaderListener::on_requested_deadline_missed(pDataReader the_reader, RequestedDeadlineMissedStatus* status)
	{

	}

	void	BuiltinSubscriptionsReaderListener::on_requested_incompatible_qos(pDataReader the_reader, RequestedIncompatibleQosStatus* status)
	{

	}

	void	BuiltinSubscriptionsReaderListener::on_subscription_matched(pDataReader the_reader, SubscriptionMatchedStatus* status)
	{

	}

	void	BuiltinSubscriptionsReaderListener::on_sample_lost(pDataReader the_reader, SampleLostStatus* status)
	{

	}
}
