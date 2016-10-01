#include "DiscoveredDataTest.h"
#include "../include/RTPS/Discovery/SPDPdiscoveredParticipantData.h"
#include "../include/RTPS/Discovery/SEDPdiscoveredPublicationData.h"
#include "../include/RTPS/Discovery/SEDPdiscoveredSubscriptionData.h"
#include "../include/RTPS/Discovery/SEDPdiscoveredTopicData.h"
#include "../include/Utility/Monitoring/Monitor.h"
#include <stdio.h>

namespace CNU_DDS
{
	void	DiscoveredDataTest()
	{
		///DiscoveredParticipantDataTest();
		//DiscoveredPublicationDataTest();
		//DiscoveredSubscriptionDataTest();
	}

	void	DiscoveredParticipantDataTest()
	{
		SPDPdiscoveredParticipantData*	sending_data	= new SPDPdiscoveredParticipantData();
		SPDPdiscoveredParticipantData*	receiving_data	= new SPDPdiscoveredParticipantData();
		ByteStream*						stream;

		Locator_t	locator_1	= {LOCATOR_KIND_UDPv4, 1111, "168.188.128.147"};
		Locator_t	locator_2	= {LOCATOR_KIND_UDPv4, 2222, "168.188.128.147"};
		Locator_t	locator_3	= {LOCATOR_KIND_UDPv4, 3333, "168.188.128.147"};
		Locator_t	locator_4	= {LOCATOR_KIND_UDPv4, 4444, "168.188.128.147"};

		sending_data->default_unicast_locator_list	= new LocatorSeq();
		sending_data->default_multicast_locator_list	= new LocatorSeq();
		sending_data->metatraffic_unicast_locator_list	= new LocatorSeq();
		sending_data->metatraffic_multicast_locator_list	= new LocatorSeq();

		sending_data->default_unicast_locator_list->insertInRear(&locator_1);
		sending_data->default_multicast_locator_list->insertInRear(&locator_2);
		sending_data->metatraffic_unicast_locator_list->insertInRear(&locator_3);
		sending_data->metatraffic_multicast_locator_list->insertInRear(&locator_4);

		sending_data->guid_prefix.host_id[0]	= 0x01;
		sending_data->guid_prefix.host_id[1]	= 0x01;
		sending_data->guid_prefix.host_id[2]	= 0x01;
		sending_data->guid_prefix.host_id[3]	= 0x01;

		sending_data->guid_prefix.app_id[0]	= 0x02;
		sending_data->guid_prefix.app_id[1]	= 0x02;
		sending_data->guid_prefix.app_id[2]	= 0x02;
		sending_data->guid_prefix.app_id[3]	= 0x02;

		sending_data->guid_prefix.counter[0]	= 0x03;
		sending_data->guid_prefix.counter[1]	= 0x03;
		sending_data->guid_prefix.counter[2]	= 0x03;
		sending_data->guid_prefix.counter[3]	= 0x03;

		sending_data->lease_duration.sec		= 10;
		sending_data->lease_duration.nanosec	= 100;

		sending_data->protocol_version.major	= 2;
		sending_data->protocol_version.minor	= 1;

		sending_data->vendor_id.vendor_id[0]	= 0x0F;
		sending_data->vendor_id.vendor_id[1]	= 0xF0;

		sending_data->available_builtin_endpoints	=  DISC_BUILTIN_ENDPOINT_PARTICIPANT_ANNOUNCER;
		sending_data->available_builtin_endpoints	+= DISC_BUILTIN_ENDPOINT_PARTICIPANT_DETECTOR;
		sending_data->available_builtin_endpoints	+= DISC_BUILTIN_ENDPOINT_PUBLICATION_ANNOUNCER;
		sending_data->available_builtin_endpoints	+= DISC_BUILTIN_ENDPOINT_PUBLICATION_DETECTOR;
		sending_data->available_builtin_endpoints	+= DISC_BUILTIN_ENDPOINT_SUBSCRIPTION_ANNOUNCER;
		sending_data->available_builtin_endpoints	+= DISC_BUILTIN_ENDPOINT_SUBSCRIPTION_DETECTOR;

		printf("Serialize(Part)\n");
		stream	= sending_data->Serialize();

		printf("Deserialize(Part)\n");
		receiving_data->Deserialize(stream);

		printf("Check up(Part)\n");

		printf("\t%s\n", (!memcmp(&sending_data->guid_prefix, &receiving_data->guid_prefix, sizeof(GuidPrefix_t)))?"SUCCESS":"FAIL");
		printf("\t%s\n", (!memcmp(&sending_data->available_builtin_endpoints, &receiving_data->available_builtin_endpoints, sizeof(4)))?"SUCCESS":"FAIL");
		printf("\t%s\n", (!memcmp(&sending_data->protocol_version, &receiving_data->protocol_version, sizeof(ProtocolVersion_t)))?"SUCCESS":"FAIL");
		printf("\t%s\n", (!memcmp(&sending_data->vendor_id, &receiving_data->vendor_id, sizeof(VendorId_t)))?"SUCCESS":"FAIL");
		printf("\t%s\n", (!memcmp(&sending_data->lease_duration, &receiving_data->lease_duration, sizeof(Duration_t)))?"SUCCESS":"FAIL");
	}

	void	DiscoveredPublicationDataTest()
	{
		SEDPdiscoveredPublicationData*	sending_data	= new SEDPdiscoveredPublicationData();
		SEDPdiscoveredPublicationData*	receiving_data	= new SEDPdiscoveredPublicationData();
		ByteStream*						stream;

		printf("\nPubTest\n");

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

		printf("Serialize(Pub)\n");
		stream	= sending_data->Serialize();

		printf("Deserialize(Pub)\n");
		receiving_data->Deserialize(stream);

		printf("Check up(Pub)\n");

		printf("\t%s\n", (!memcmp(&sending_data->deadline, &receiving_data->deadline, sizeof(DeadlineQosPolicy)))?"SUCCESS":"FAIL");
		printf("\t%s\n", (!memcmp(&sending_data->destination_order, &receiving_data->destination_order, sizeof(DestinationOrderQosPolicy)))?"SUCCESS":"FAIL");
		printf("\t%s\n", (!memcmp(&sending_data->durability, &receiving_data->durability, sizeof(DurabilityQosPolicy)))?"SUCCESS":"FAIL");
		printf("\t%s\n", (!memcmp(&sending_data->durability_service, &receiving_data->durability_service, sizeof(DurabilityServiceQosPolicy)))?"SUCCESS":"FAIL");
		printf("\t%s\n", (!memcmp(&sending_data->latency_budget, &receiving_data->latency_budget, sizeof(LatencyBudgetQosPolicy)))?"SUCCESS":"FAIL");
		printf("\n");
		printf("\t%s\n", (!memcmp(&sending_data->lifespan, &receiving_data->lifespan, sizeof(LifespanQosPolicy)))?"SUCCESS":"FAIL");
		printf("\t%s\n", (!memcmp(&sending_data->liveliness, &receiving_data->liveliness, sizeof(LivelinessQosPolicy)))?"SUCCESS":"FAIL");
		printf("\t%s\n", (!memcmp(&sending_data->ownership, &receiving_data->ownership, sizeof(OwnershipQosPolicy)))?"SUCCESS":"FAIL");
		printf("\t%s\n", (!memcmp(&sending_data->ownership_strength, &receiving_data->ownership_strength, sizeof(OwnershipStrengthQosPolicy)))?"SUCCESS":"FAIL");
		printf("\t%s\n", (!memcmp(&sending_data->presentation, &receiving_data->presentation, sizeof(PresentationQosPolicy)))?"SUCCESS":"FAIL");
	}

	void	DiscoveredSubscriptionDataTest()
	{
		SEDPdiscoveredSubscriptionData*	sending_data	= new SEDPdiscoveredSubscriptionData();
		SEDPdiscoveredSubscriptionData*	receiving_data	= new SEDPdiscoveredSubscriptionData;
		ByteStream*						stream;

		printf("\nSubTest\n");

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

		printf("Serialize(Sub)\n");
		stream	= sending_data->Serialize();

		printf("Deserialize(Sub)\n");
		receiving_data->Deserialize(stream);

		printf("Check up(Sub)\n");

		printf("\t%s\n", (!memcmp(&sending_data->deadline, &receiving_data->deadline, sizeof(DeadlineQosPolicy)))?"SUCCESS":"FAIL");
		printf("\t%s\n", (!memcmp(&sending_data->destination_order, &receiving_data->destination_order, sizeof(DestinationOrderQosPolicy)))?"SUCCESS":"FAIL");
		printf("\t%s\n", (!memcmp(&sending_data->durability, &receiving_data->durability, sizeof(DurabilityQosPolicy)))?"SUCCESS":"FAIL");
		printf("\t%s\n", (!memcmp(&sending_data->durability_service, &receiving_data->durability_service, sizeof(DurabilityServiceQosPolicy)))?"SUCCESS":"FAIL");
		printf("\t%s\n", (!memcmp(&sending_data->latency_budget, &receiving_data->latency_budget, sizeof(LatencyBudgetQosPolicy)))?"SUCCESS":"FAIL");
		printf("\n");
		printf("\t%s\n", (!memcmp(&sending_data->lifespan, &receiving_data->lifespan, sizeof(LifespanQosPolicy)))?"SUCCESS":"FAIL");
		printf("\t%s\n", (!memcmp(&sending_data->liveliness, &receiving_data->liveliness, sizeof(LivelinessQosPolicy)))?"SUCCESS":"FAIL");
		printf("\t%s\n", (!memcmp(&sending_data->ownership, &receiving_data->ownership, sizeof(OwnershipQosPolicy)))?"SUCCESS":"FAIL");
		printf("\t%s\n", (!memcmp(&sending_data->presentation, &receiving_data->presentation, sizeof(PresentationQosPolicy)))?"SUCCESS":"FAIL");
	}

	void	DiscoveredTopicDataTest()
	{

	}

	void	DiscoveredDataInterfaceTest()
	{

	}

	void	DiscoveredDataReceivingListener()
	{

	}
}
