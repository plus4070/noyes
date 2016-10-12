#include "BehaviorTest.h"

#include "../include/Utility/PlatformOrientedInterface.h"
#include "../include/RTPS/Behavior/StatelessWriter.h"
#include "../include/RTPS/Behavior/StatelessReader.h"
#include "../include/RTPS/Behavior/StatefulWriter.h"
#include "../include/RTPS/Behavior/StatefulReader.h"
#include "../include/Utility/Network/TransceiverManager.h"
#include "../include/Utility/Timer/DynamicTimer.h"
#include "../include/RTPS/Discovery/SPDPdiscoveredParticipantData.h"
#include "../include/RTPS/Discovery/SEDPdiscoveredPublicationData.h"
#include "../include/RTPS/Discovery/SEDPdiscoveredSubscriptionData.h"

#include "DCPS/TestListener.h"
#include "../include/DCPS/Domain/DomainParticipant.h"
#include "../include/DCPS/Subscription/Subscriber.h"
#include "../include/DCPS/Subscription/DataReader.h"

#include <stdio.h>
#include <string.h>

namespace CNU_DDS
{
	void	BehaviorTest()
	{
		DynamicTimer*	timer		= DynamicTimer::getDynamicTimerInstance();
		int i;

		printf("[1] StatelessWriter\n");
		printf("[2] StatelessReader\n");
		printf("[3] StatefulWriter\n");
		printf("[4] StatefulReader\n");
		printf("select>");
		scanf("%d", &i);

		switch(i)
		{
		case 1:
			StatelessWriterTest();
			break;
		case 2:
			StatelessReaderTest();
			break;
		case 3:
			StatefulWriterTest();
			break;
		case 4:
			StatefulReaderTest();
			break;
		default:
			break;
		}
	}

	void	StatelessWriterTest()
	{
		StatelessWriter*	writer		= NULL;
		Locator_t			writer_locator	= {LOCATOR_KIND_UDPv4, 1235, "168.188.128.147"};
		Locator_t			locator_1	= {LOCATOR_KIND_UDPv4, 1234, "239.255.0.1"};
		//Locator_t			locator_2	= {LOCATOR_KIND_UDPv4, 5678, "168.188.128.147"};
		TransceiverManager*	tm			= TransceiverManager::getManagerInstance();
		Transceiver*		tr;
		unsigned long		transceiver_id;
		GUID_t				guid;

		ByteStream*			a_data		= new ByteStream();
		unsigned char		indata[5]	= "test";

		guid.guid_prefix.host_id[0]	= 0x00;
		guid.guid_prefix.host_id[1]	= 0x01;
		guid.guid_prefix.host_id[2]	= 0x02;
		guid.guid_prefix.host_id[3]	= 0x03;

		guid.guid_prefix.app_id[0]	= 0x00;
		guid.guid_prefix.app_id[1]	= 0x01;
		guid.guid_prefix.app_id[2]	= 0x02;
		guid.guid_prefix.app_id[3]	= 0x03;

		guid.guid_prefix.counter[0]	= 0x00;
		guid.guid_prefix.counter[1]	= 0x01;
		guid.guid_prefix.counter[2]	= 0x02;
		guid.guid_prefix.counter[3]	= 0x03;

		guid.entity_id	= ENTITYID_SPDP_BUILTIN_PARTICIPANT_WRITER;

		memcpy(&transceiver_id, &guid.entity_id, 4);
		printf("Transceiver ID : %08X\n", transceiver_id);
		a_data->setData(BYTE_ORDERING_TYPE_LITTLE_ENDIAN, indata, 5);

		tr	= tm->registerTransceiver(writer_locator, transceiver_id);

		writer	= new StatelessWriter(guid, RELIABILITY_KIND_BEST_EFFORT, Duration_t(0,0));

		tr->regist(writer);
		printf("Transceiver : %08X\n", tr);

		writer->reader_locator_add(locator_1);
		//writer->reader_locator_add(locator_2);

		native_sleep(500);
		writer->new_change(CHANGE_KIND_ALIVE, (ByteStream*)create_PDP_message(), 0);
		//writer->new_change(CHANGE_KIND_ALIVE, a_data, 0);
		writer->unsent_changes_reset();
		writer->unsent_changes_reset();
		//writer->new_change(CHANGE_KIND_ALIVE, a_data, 0);
		//writer->new_change(CHANGE_KIND_ALIVE, a_data, 0);

		native_sleep(500);
	}

	void	StatelessReaderTest()
	{
		StatelessReader*	reader		= NULL;
		//Locator_t			locator_1	= {LOCATOR_KIND_UDPv4, 1234, "239.255.0.1"};
		Locator_t			locator_1	= {LOCATOR_KIND_UDPv4, 1234, "168.188.128.147"};
		TransceiverManager*	tm			= TransceiverManager::getManagerInstance();
		Transceiver*		tr;
		unsigned long		transceiver_id;
		GUID_t				guid;
		int					i, recv_count	= 0;

		pCacheChange		change;
		ByteStream*			a_data		= new ByteStream();

		guid.guid_prefix.host_id[0]	= 0x00;
		guid.guid_prefix.host_id[1]	= 0x01;
		guid.guid_prefix.host_id[2]	= 0x02;
		guid.guid_prefix.host_id[3]	= 0x03;

		guid.guid_prefix.app_id[0]	= 0x00;
		guid.guid_prefix.app_id[1]	= 0x01;
		guid.guid_prefix.app_id[2]	= 0x02;
		guid.guid_prefix.app_id[3]	= 0x03;

		guid.guid_prefix.counter[0]	= 0x00;
		guid.guid_prefix.counter[1]	= 0x01;
		guid.guid_prefix.counter[2]	= 0x02;
		guid.guid_prefix.counter[3]	= 0x03;

		guid.entity_id	= ENTITYID_SPDP_BUILTIN_PARTICIPANT_READER;

		memcpy(&transceiver_id, &guid.entity_id, 4);
		printf("Transceiver ID : %08X\n", transceiver_id);

		tr	= tm->registerTransceiver(locator_1, transceiver_id);

		reader	= new StatelessReader(guid, RELIABILITY_KIND_BEST_EFFORT);

		tr->regist(reader);
		printf("Transceiver : %08X (%08X)\n", tr, reader);

		for(i=0; i<15 && recv_count < 6; i++)
		{
			native_sleep(1000);
			printf("check(%d)\n", recv_count);
			if(reader->reader_cache->changes->getSize() > 0)
			{
				change	= reader->reader_cache->changes->popFirst();
				printf("recv_a_data : %s\n", change->data_value->getData());
				recv_count++;
			}
		}
	}

	void	StatefulWriterTest()
	{
		StatefulWriter*		writer		= NULL;
		ReaderProxy*		proxy_1		= NULL;

		Locator_t			writer_locator	= {LOCATOR_KIND_UDPv4, 1235, "168.188.128.147"};
		Locator_t			locator_1		= {LOCATOR_KIND_UDPv4, 1234, "239.255.0.1"};

		TransceiverManager*	tm			= TransceiverManager::getManagerInstance();
		Transceiver*		tr;
		unsigned long		transceiver_id;
		GUID_t				guid;

		ByteStream*			a_data		= new ByteStream();
		unsigned char		indata[5]	= "test";

		SequenceNumber_t	sn;
		ChangeForReader*	cfr;
		sn.high	= 0;
		sn.low	= 1;

		guid.guid_prefix.host_id[0]	= 0x00;
		guid.guid_prefix.host_id[1]	= 0x01;
		guid.guid_prefix.host_id[2]	= 0x02;
		guid.guid_prefix.host_id[3]	= 0x03;

		guid.guid_prefix.app_id[0]	= 0x00;
		guid.guid_prefix.app_id[1]	= 0x01;
		guid.guid_prefix.app_id[2]	= 0x02;
		guid.guid_prefix.app_id[3]	= 0x03;

		guid.guid_prefix.counter[0]	= 0x00;
		guid.guid_prefix.counter[1]	= 0x01;
		guid.guid_prefix.counter[2]	= 0x02;
		guid.guid_prefix.counter[3]	= 0x03;

		guid.entity_id	= ENTITYID_SEDP_BUILTIN_PUBLICATIONS_WRITER;

		memcpy(&transceiver_id, &guid.entity_id, 4);
		printf("Transceiver ID : %08X\n", transceiver_id);
		a_data->setData(BYTE_ORDERING_TYPE_LITTLE_ENDIAN, indata, 5);

		tr	= tm->registerTransceiver(writer_locator, transceiver_id);

		writer	= new StatefulWriter(guid, RELIABILITY_KIND_RELIABLE, true);
		tr->regist(writer);

		proxy_1		= new ReaderProxy((unsigned long)writer);
		proxy_1->remote_reader_guid.guid_prefix	= guid.guid_prefix;
		proxy_1->remote_reader_guid.entity_id	= ENTITYID_SEDP_BUILTIN_PUBLICATIONS_READER;
		proxy_1->unicast_locator_list->insertInRear(&locator_1);

		writer->matched_reader_add(proxy_1);

		native_sleep(500);
		writer->new_change(CHANGE_KIND_ALIVE, (ByteStream*)create_Pub_EDP_message(), 0);
		printf("send\n");
		native_sleep(500);
		writer->new_change(CHANGE_KIND_ALIVE, (ByteStream*)create_Pub_EDP_message(), 0);
		printf("send\n");
		native_sleep(500);
		writer->new_change(CHANGE_KIND_ALIVE, (ByteStream*)create_Pub_EDP_message(), 0);
		printf("send\n");
		native_sleep(500);
		writer->new_change(CHANGE_KIND_ALIVE, (ByteStream*)create_Pub_EDP_message(), 0);
		printf("send\n");
		native_sleep(500);
		writer->new_change(CHANGE_KIND_ALIVE, (ByteStream*)create_Pub_EDP_message(), 0);
		printf("send\n");
		native_sleep(500);
		writer->new_change(CHANGE_KIND_ALIVE, (ByteStream*)create_Pub_EDP_message(), 0);
		printf("send\n");
		native_sleep(500);
		writer->new_change(CHANGE_KIND_ALIVE, (ByteStream*)create_Pub_EDP_message(), 0);
		printf("send\n");
		native_sleep(500);
		writer->new_change(CHANGE_KIND_ALIVE, (ByteStream*)create_Pub_EDP_message(), 0);
		printf("send\n");
	}

	void	StatefulReaderTest()
	{
		StatefulReader*		reader		= NULL;
		WriterProxy*		proxy_1		= NULL;

		Locator_t			reader_locator	= {LOCATOR_KIND_UDPv4, 1234, "168.188.128.147"};
		Locator_t			locator_1		= {LOCATOR_KIND_UDPv4, 1235, "239.255.0.1"};

		TransceiverManager*	tm			= TransceiverManager::getManagerInstance();
		Transceiver*		tr;
		unsigned long		transceiver_id;
		GUID_t				guid;

		ByteStream*			a_data		= new ByteStream();
		unsigned char		indata[5]	= "test";

		SequenceNumber_t	sn;
		ChangeFromWriter*	cfr;

		Subscriber*			subscriber;
		DataReader*			datareader;
		TestListener*		test_listener;

		sn.high	= 0;
		sn.low	= 5;

		guid.guid_prefix.host_id[0]	= 0x00;
		guid.guid_prefix.host_id[1]	= 0x01;
		guid.guid_prefix.host_id[2]	= 0x02;
		guid.guid_prefix.host_id[3]	= 0x03;

		guid.guid_prefix.app_id[0]	= 0x00;
		guid.guid_prefix.app_id[1]	= 0x01;
		guid.guid_prefix.app_id[2]	= 0x02;
		guid.guid_prefix.app_id[3]	= 0x03;

		guid.guid_prefix.counter[0]	= 0x00;
		guid.guid_prefix.counter[1]	= 0x01;
		guid.guid_prefix.counter[2]	= 0x02;
		guid.guid_prefix.counter[3]	= 0x03;

		guid.entity_id	= ENTITYID_SEDP_BUILTIN_PUBLICATIONS_READER;

		memcpy(&transceiver_id, &guid.entity_id, 4);
		printf("Transceiver ID : %08X\n", transceiver_id);
		a_data->setData(BYTE_ORDERING_TYPE_LITTLE_ENDIAN, indata, 5);

		tr	= tm->registerTransceiver(reader_locator, transceiver_id);

		subscriber		= new Subscriber();
		datareader		= new DataReader();
		test_listener	= new TestListener();

		datareader->related_subscriber	= subscriber;
		datareader->set_listener(test_listener, 0xFFFFFFFF);

		reader			= new StatefulReader(guid, RELIABILITY_KIND_RELIABLE);
		datareader->related_rtps_reader	= reader;
		datareader->related_subscriber	= subscriber;
		reader->related_dcps_reader		= datareader;
		tr->regist(reader);

		proxy_1		= new WriterProxy((unsigned long)reader);
		proxy_1->remote_writer_guid.guid_prefix	= guid.guid_prefix;
		proxy_1->remote_writer_guid.entity_id	= ENTITYID_SEDP_BUILTIN_PUBLICATIONS_WRITER;
		proxy_1->unicast_locator_list->insertInRear(&locator_1);

		reader->matched_writer_add(proxy_1);

		for(int i=0; i<10; i++)
		{
			//printf("check(%2d)\n", i);
			//printf("reader_cache->get_seq_num_max = %d\n", reader->reader_cache->get_seq_num_max().low);
			native_sleep(1000);
		}

		//SEDPdiscoveredPublicationData*	recv_data	= new SEDPdiscoveredPublicationData();
		//recv_data->Deserialize(reader->reader_cache->changes->getFirst()->data_value);
		//printf("lifespan(%08X, %08X)\n", recv_data->lifespan.duration.sec, recv_data->lifespan.duration.nanosec);
	}

	void*	create_PDP_message()
	{
		SPDPdiscoveredParticipantData	sending_data;

		ByteStream*		stream;

		Locator_t	locator_1	= {LOCATOR_KIND_UDPv4, 1111, "168.188.128.147"};
		Locator_t	locator_2	= {LOCATOR_KIND_UDPv4, 2222, "168.188.128.147"};
		Locator_t	locator_3	= {LOCATOR_KIND_UDPv4, 3333, "168.188.128.147"};
		Locator_t	locator_4	= {LOCATOR_KIND_UDPv4, 4444, "168.188.128.147"};

		sending_data.default_unicast_locator_list	= new LocatorSeq();
		sending_data.default_multicast_locator_list	= new LocatorSeq();
		sending_data.metatraffic_unicast_locator_list	= new LocatorSeq();
		sending_data.metatraffic_multicast_locator_list	= new LocatorSeq();

		sending_data.default_unicast_locator_list->insertInRear(&locator_1);
		sending_data.default_multicast_locator_list->insertInRear(&locator_2);
		sending_data.metatraffic_unicast_locator_list->insertInRear(&locator_3);
		sending_data.metatraffic_multicast_locator_list->insertInRear(&locator_4);

		sending_data.guid_prefix.host_id[0]	= 0x01;
		sending_data.guid_prefix.host_id[1]	= 0x01;
		sending_data.guid_prefix.host_id[2]	= 0x01;
		sending_data.guid_prefix.host_id[3]	= 0x01;

		sending_data.guid_prefix.app_id[0]	= 0x02;
		sending_data.guid_prefix.app_id[1]	= 0x02;
		sending_data.guid_prefix.app_id[2]	= 0x02;
		sending_data.guid_prefix.app_id[3]	= 0x02;

		sending_data.guid_prefix.counter[0]	= 0x03;
		sending_data.guid_prefix.counter[1]	= 0x03;
		sending_data.guid_prefix.counter[2]	= 0x03;
		sending_data.guid_prefix.counter[3]	= 0x03;

		sending_data.lease_duration.sec		= 10;
		sending_data.lease_duration.nanosec	= 100;

		sending_data.protocol_version.major	= 2;
		sending_data.protocol_version.minor	= 1;

		sending_data.vendor_id.vendor_id[0]	= 0x0F;
		sending_data.vendor_id.vendor_id[1]	= 0xF0;

		sending_data.available_builtin_endpoints	=  DISC_BUILTIN_ENDPOINT_PARTICIPANT_ANNOUNCER;
		sending_data.available_builtin_endpoints	+= DISC_BUILTIN_ENDPOINT_PARTICIPANT_DETECTOR;
		sending_data.available_builtin_endpoints	+= DISC_BUILTIN_ENDPOINT_PUBLICATION_ANNOUNCER;
		sending_data.available_builtin_endpoints	+= DISC_BUILTIN_ENDPOINT_PUBLICATION_DETECTOR;
		sending_data.available_builtin_endpoints	+= DISC_BUILTIN_ENDPOINT_SUBSCRIPTION_ANNOUNCER;
		sending_data.available_builtin_endpoints	+= DISC_BUILTIN_ENDPOINT_SUBSCRIPTION_DETECTOR;

		stream	= sending_data.Serialize();

		return stream;
	}

	void*	create_Pub_EDP_message()
	{
		SEDPdiscoveredPublicationData*	sending_data	= new SEDPdiscoveredPublicationData();
		ByteStream*						stream;

		sending_data->remote_writer_guid.guid_prefix.host_id[0]	= 0x01;
		sending_data->remote_writer_guid.guid_prefix.host_id[1]	= 0x01;
		sending_data->remote_writer_guid.guid_prefix.host_id[2]	= 0x01;
		sending_data->remote_writer_guid.guid_prefix.host_id[3]	= 0x01;

		sending_data->remote_writer_guid.guid_prefix.app_id[0]	= 0x02;
		sending_data->remote_writer_guid.guid_prefix.app_id[1]	= 0x02;
		sending_data->remote_writer_guid.guid_prefix.app_id[2]	= 0x02;
		sending_data->remote_writer_guid.guid_prefix.app_id[3]	= 0x02;

		sending_data->remote_writer_guid.guid_prefix.counter[0]	= 0x03;
		sending_data->remote_writer_guid.guid_prefix.counter[1]	= 0x03;
		sending_data->remote_writer_guid.guid_prefix.counter[2]	= 0x03;
		sending_data->remote_writer_guid.guid_prefix.counter[3]	= 0x03;

		sending_data->remote_writer_guid.entity_id.entity_key[0]	= 0x04;
		sending_data->remote_writer_guid.entity_id.entity_key[1]	= 0x04;
		sending_data->remote_writer_guid.entity_id.entity_key[2]	= 0x04;
		sending_data->remote_writer_guid.entity_id.entity_kind	= 0x05;

		sending_data->topic_name	= "SENDING TOPIC";

		sending_data->type_name	= "SENDING TYPE";

		sending_data->deadline.period.sec		= 0x01;
		sending_data->deadline.period.nanosec	= 0x02;

		sending_data->destination_order.kind		= BY_SOURCE_TIMESTAMP_DESTINATIONORDER_QOS;

		sending_data->durability.kind			= TRANSIENT_DURABILITY_QOS;

		sending_data->durability_service.history_depth	= 0x04;
		sending_data->durability_service.history_kind	= KEEP_ALL_HISTORY_QOS;
		sending_data->durability_service.max_instances	= 0x02;
		sending_data->durability_service.max_samples		= 0x08;
		sending_data->durability_service.max_samples_per_instance	= 0x0F;

		sending_data->latency_budget.duration.sec		= 0x04;
		sending_data->latency_budget.duration.nanosec	= 0x08;

		sending_data->lifespan.duration.sec		= 0x03;
		sending_data->lifespan.duration.nanosec	= 0x06;

		sending_data->liveliness.kind					= MANUAL_BY_TOPIC_LIVELINESS_QOS;
		sending_data->liveliness.lease_duration.sec		= 0x01;
		sending_data->liveliness.lease_duration.nanosec	= 0x40;

		sending_data->ownership.kind						= SHARED_OWNERSHIP_QOS;

		sending_data->ownership_strength.value			= 0x70;

		sending_data->presentation.access_scope			= GROUP_PRESENTATION_QOS;
		sending_data->presentation.coherent_access		= false;
		sending_data->presentation.ordered_access		= true;

		stream	= sending_data->Serialize();
		delete(sending_data);

		return stream;
	}

	void*	create_Sub_EDP_message()
	{
		SEDPdiscoveredSubscriptionData*	sending_data	= new SEDPdiscoveredSubscriptionData();
		ByteStream*						stream;

		sending_data->remote_reader_guid.guid_prefix.host_id[0]	= 0x01;
		sending_data->remote_reader_guid.guid_prefix.host_id[1]	= 0x01;
		sending_data->remote_reader_guid.guid_prefix.host_id[2]	= 0x01;
		sending_data->remote_reader_guid.guid_prefix.host_id[3]	= 0x01;

		sending_data->remote_reader_guid.guid_prefix.app_id[0]	= 0x02;
		sending_data->remote_reader_guid.guid_prefix.app_id[1]	= 0x02;
		sending_data->remote_reader_guid.guid_prefix.app_id[2]	= 0x02;
		sending_data->remote_reader_guid.guid_prefix.app_id[3]	= 0x02;

		sending_data->remote_reader_guid.guid_prefix.counter[0]	= 0x03;
		sending_data->remote_reader_guid.guid_prefix.counter[1]	= 0x03;
		sending_data->remote_reader_guid.guid_prefix.counter[2]	= 0x03;
		sending_data->remote_reader_guid.guid_prefix.counter[3]	= 0x03;

		sending_data->remote_reader_guid.entity_id.entity_key[0]	= 0x04;
		sending_data->remote_reader_guid.entity_id.entity_key[1]	= 0x04;
		sending_data->remote_reader_guid.entity_id.entity_key[2]	= 0x04;
		sending_data->remote_reader_guid.entity_id.entity_kind	= 0x05;

		sending_data->topic_name	= "SENDING TOPIC";

		sending_data->type_name	= "SENDING TYPE";

		sending_data->deadline.period.sec		= 0x01;
		sending_data->deadline.period.nanosec	= 0x02;

		sending_data->destination_order.kind		= BY_SOURCE_TIMESTAMP_DESTINATIONORDER_QOS;

		sending_data->durability.kind			= TRANSIENT_DURABILITY_QOS;

		sending_data->durability_service.history_depth	= 0x04;
		sending_data->durability_service.history_kind	= KEEP_ALL_HISTORY_QOS;
		sending_data->durability_service.max_instances	= 0x02;
		sending_data->durability_service.max_samples		= 0x08;
		sending_data->durability_service.max_samples_per_instance	= 0x0F;

		sending_data->latency_budget.duration.sec		= 0x04;
		sending_data->latency_budget.duration.nanosec	= 0x08;

		sending_data->lifespan.duration.sec		= 0x03;
		sending_data->lifespan.duration.nanosec	= 0x06;

		sending_data->liveliness.kind					= MANUAL_BY_TOPIC_LIVELINESS_QOS;
		sending_data->liveliness.lease_duration.sec		= 0x01;
		sending_data->liveliness.lease_duration.nanosec	= 0x40;

		sending_data->ownership.kind						= SHARED_OWNERSHIP_QOS;


		sending_data->presentation.access_scope			= GROUP_PRESENTATION_QOS;
		sending_data->presentation.coherent_access		= false;
		sending_data->presentation.ordered_access		= true;

		stream	= sending_data->Serialize();
		delete(sending_data);

		return stream;
	}
}
