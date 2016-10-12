#include "../../../include/DCPS/BuiltinEntity/BuiltinParticipantsReaderListener.h"
#include "../../../include/DCPS/Domain/DomainParticipant.h"
#include "../../../include/DCPS/Publication/Publisher.h"
#include "../../../include/DCPS/Publication/DataWriter.h"
#include "../../../include/DCPS/Subscription/Subscriber.h"
#include "../../../include/DCPS/Subscription/DataReader.h"

#include "../../../include/RTPS/Discovery/SPDPdiscoveredParticipantData.h"
#include "../../../include/RTPS/Structure/Participant.h"
#include "../../../include/RTPS/Structure/CacheChange.h"
#include "../../../include/RTPS/Behavior/StatelessReader.h"
#include "../../../include/RTPS/Behavior/StatelessWriter.h"
#include "../../../include/RTPS/Behavior/StatefulReader.h"
#include "../../../include/RTPS/Behavior/StatefulWriter.h"

#include <stdio.h>

namespace CNU_DDS
{
	BuiltinParticipantsReaderListener::BuiltinParticipantsReaderListener()
	{

	}

	BuiltinParticipantsReaderListener::~BuiltinParticipantsReaderListener()
	{

	}

	void	BuiltinParticipantsReaderListener::on_data_available(pDataReader the_reader)
	{
		SPDPdiscoveredParticipantData*	recv_part		= NULL;
		StatelessReader*				reader			= (StatelessReader*)the_reader->related_rtps_reader;
		StatefulWriter*					edp_writer		= NULL;
		StatefulReader*					edp_reader		= NULL;
		pCacheChange					change			= NULL;
		DataWriter*						writer			= NULL;
		DataReader*						builtin_reader	= NULL;
		ReaderProxy*					reader_proxy	= NULL;
		WriterProxy*					writer_proxy	= NULL;

		while(reader->reader_cache->changes->getSize())
		{
			change		= reader->reader_cache->changes->popFirst();

			recv_part	= new SPDPdiscoveredParticipantData();
			recv_part->Deserialize(change->data_value);

			ParticipantProxy*	proxy	= recv_part->getParticipantProxy();

			if(the_reader->related_participant->related_rtps_participant->add_new_participant_proxy(recv_part->getParticipantProxy()))
			{
				writer	= the_reader->related_participant->get_builtin_publisher()->lookup_datawriter("BuiltinPublicationsDiscovery");
				if(writer)
				{
					edp_writer	= (StatefulWriter*)writer->related_rtps_writer;
					reader_proxy	= new ReaderProxy((unsigned long)edp_writer, 100);
					//reader_proxy	= new ReaderProxy((unsigned long)edp_writer);
					reader_proxy->remote_reader_guid.guid_prefix	= recv_part->guid_prefix;
					reader_proxy->remote_reader_guid.entity_id		= ENTITYID_SEDP_BUILTIN_PUBLICATIONS_READER;

					if(proxy->metatraffic_unicast_locator_list->getSize())
					{
						reader_proxy->unicast_locator_list->insertInRear(proxy->metatraffic_unicast_locator_list->getFirst());
					}

					((StatefulWriter*)writer->related_rtps_writer)->matched_reader_add(reader_proxy);
					reader_proxy->start();
				}

				builtin_reader	= the_reader->related_participant->get_builtin_subscriber()->lookup_datareader("BuiltinPublicationsDiscovery");
				{
					if(builtin_reader)
					{
						edp_reader	= (StatefulReader*)builtin_reader->related_rtps_reader;
						writer_proxy	= new WriterProxy((unsigned long)edp_reader, 100);
						//writer_proxy	= new WriterProxy((unsigned long)edp_reader);
						writer_proxy->remote_writer_guid.guid_prefix	= recv_part->guid_prefix;
						writer_proxy->remote_writer_guid.entity_id		= ENTITYID_SEDP_BUILTIN_PUBLICATIONS_WRITER;

						if(proxy->metatraffic_unicast_locator_list->getSize())
						{
							writer_proxy->unicast_locator_list->insertInRear(proxy->metatraffic_unicast_locator_list->getFirst());
						}

						edp_reader->matched_writer_add(writer_proxy);
						writer_proxy->start();
					}
				}

				writer	= the_reader->related_participant->get_builtin_publisher()->lookup_datawriter("BuiltinSubscriptionsDiscovery");
				if(writer)
				{
					edp_writer	= (StatefulWriter*)writer->related_rtps_writer;

					reader_proxy	= new ReaderProxy((unsigned long)edp_writer, 100);
					//reader_proxy	= new ReaderProxy((unsigned long)edp_writer);
					reader_proxy->remote_reader_guid.guid_prefix	= recv_part->guid_prefix;
					reader_proxy->remote_reader_guid.entity_id		= ENTITYID_SEDP_BUILTIN_SUBSCRIPTIONS_READER;

					if(proxy->metatraffic_unicast_locator_list->getSize())
					{
						reader_proxy->unicast_locator_list->insertInRear(proxy->metatraffic_unicast_locator_list->getFirst());
					}

					((StatefulWriter*)writer->related_rtps_writer)->matched_reader_add(reader_proxy);
					reader_proxy->start();
				}

				builtin_reader	= the_reader->related_participant->get_builtin_subscriber()->lookup_datareader("BuiltinSubscriptionsDiscovery");
				{
					if(builtin_reader)
					{
						edp_reader	= (StatefulReader*)builtin_reader->related_rtps_reader;
						writer_proxy	= new WriterProxy((unsigned long)edp_reader, 100);
						//writer_proxy	= new WriterProxy((unsigned long)edp_reader);
						writer_proxy->remote_writer_guid.guid_prefix	= recv_part->guid_prefix;
						writer_proxy->remote_writer_guid.entity_id		= ENTITYID_SEDP_BUILTIN_SUBSCRIPTIONS_WRITER;

						if(proxy->metatraffic_unicast_locator_list->getSize())
						{
							writer_proxy->unicast_locator_list->insertInRear(proxy->metatraffic_unicast_locator_list->getFirst());
						}

						edp_reader->matched_writer_add(writer_proxy);
						writer_proxy->start();
					}
				}
			}
			else
			{
				delete(recv_part);
			}
		}
	}

	void	BuiltinParticipantsReaderListener::on_sample_rejected(pDataReader the_reader, SampleRejectedStatus* status)
	{

	}

	void	BuiltinParticipantsReaderListener::on_liveliness_changed(pDataReader the_reader, LivelinessChangedStatus* status)
	{

	}

	void	BuiltinParticipantsReaderListener::on_requested_deadline_missed(pDataReader the_reader, RequestedDeadlineMissedStatus* status)
	{

	}

	void	BuiltinParticipantsReaderListener::on_requested_incompatible_qos(pDataReader the_reader, RequestedIncompatibleQosStatus* status)
	{

	}

	void	BuiltinParticipantsReaderListener::on_subscription_matched(pDataReader the_reader, SubscriptionMatchedStatus* status)
	{

	}

	void	BuiltinParticipantsReaderListener::on_sample_lost(pDataReader the_reader, SampleLostStatus* status)
	{

	}
}
