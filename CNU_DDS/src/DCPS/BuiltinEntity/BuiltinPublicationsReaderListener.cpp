#include "../../../include/DCPS/BuiltinEntity/BuiltinPublicationsReaderListener.h"
#include "../../../include/DCPS/Domain/DomainParticipant.h"
#include "../../../include/DCPS/Subscription/DataReader.h"

#include "../../../include/RTPS/Discovery/SEDPdiscoveredPublicationData.h"
#include "../../../include/RTPS/Structure/Participant.h"
#include "../../../include/RTPS/Structure/CacheChange.h"
#include "../../../include/RTPS/Behavior/StatefulReader.h"

#include <stdio.h>

namespace CNU_DDS
{
	BuiltinPublicationsReaderListener::BuiltinPublicationsReaderListener()
	{

	}

	BuiltinPublicationsReaderListener::~BuiltinPublicationsReaderListener()
	{

	}

	void	BuiltinPublicationsReaderListener::on_data_available(pDataReader the_reader)
	{
		StatefulReader*					rtps_reader		= (StatefulReader*)the_reader->related_rtps_reader;
		Participant*					rtps_part		= the_reader->related_participant->related_rtps_participant;
		ParticipantProxy*				recv_part		= NULL;
		SEDPdiscoveredPublicationData*	recv_edp		= NULL;
		pCacheChange					change			= NULL;
		Locator_t*						locator;

//printf("Pub_ReaderListener::on_data_available()\n");
//printf("\trecv_reader->reader_cache->changes->getSize() == %d\n", rtps_reader->reader_cache->changes->getSize());
		while(rtps_reader->reader_cache->changes->getSize() > 0)
		{
			recv_edp	= new SEDPdiscoveredPublicationData();
			change		= rtps_reader->reader_cache->changes->popFirst();

			recv_edp->Deserialize(change->data_value);

			recv_part	= rtps_part->get_remote_participant(recv_edp->remote_writer_guid.guid_prefix);
//printf("\trecv_part(%08X)\n", recv_part);

			if(recv_part)
			{
				/* Insert a default_unicast_locator from recv_participant to recv_edp */
				/* and others? */
//printf("\tcreate_new_WriterProxy()\n");
				locator	= new Locator_t;
				memcpy(locator, recv_part->default_unicast_locator_list->getFirst(), sizeof(Locator_t));
				recv_edp->unicast_locator_list->insertInRear(locator);
				the_reader->related_participant->_remote_datawriter_is_discovered(recv_edp);
			}
		}
	}

	void	BuiltinPublicationsReaderListener::on_sample_rejected(pDataReader the_reader, SampleRejectedStatus* status)
	{

	}

	void	BuiltinPublicationsReaderListener::on_liveliness_changed(pDataReader the_reader, LivelinessChangedStatus* status)
	{

	}

	void	BuiltinPublicationsReaderListener::on_requested_deadline_missed(pDataReader the_reader, RequestedDeadlineMissedStatus* status)
	{

	}

	void	BuiltinPublicationsReaderListener::on_requested_incompatible_qos(pDataReader the_reader, RequestedIncompatibleQosStatus* status)
	{

	}

	void	BuiltinPublicationsReaderListener::on_subscription_matched(pDataReader the_reader, SubscriptionMatchedStatus* status)
	{

	}

	void	BuiltinPublicationsReaderListener::on_sample_lost(pDataReader the_reader, SampleLostStatus* status)
	{

	}
}
