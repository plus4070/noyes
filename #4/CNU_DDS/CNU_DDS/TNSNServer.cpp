#include "TNSNheader.h"
#include "TNSNServer.h"
#include "TNSNController.h"

#include "include/DCPS/Domain/DomainParticipant.h"
#include "include/RTPS/Discovery/SEDPdiscoveredPublicationData.h"
#include "include/RTPS/Discovery/SEDPdiscoveredSubscriptionData.h"
#include "include/Utility/Monitoring/Monitor.h"

#include "include/DCPS/Publication/DataWriter.h"
#include "include/DCPS/Domain/DomainParticipant.h"
#include "include/DCPS/Publication/Publisher.h"
#include "application/DCPS/ShapeTest.h"

namespace CNU_DDS
{
	CRITICAL_SECTION	cs;
	HANDLE				recvThread;
	HANDLE				saveThread;
	deque<PDD_NODE>		recvQueue;
	int					TNS_TYPE;
	TNSNController		*controller;
	Monitor				*tnsMonitor;
	DomainParticipant	*tnsDomainPart;

	void InitialDomainForTNS() {
		unsigned long	domain_id = 0;

		DomainParticipantQos*	part_qos;
		DomainParticipant*		part;
		Topic*					topic;

		PublisherQos*		pub_qos;
		Publisher*			pub;
		DataWriterQos*		writer_qos;
		DataWriter*			writer;

		Monitor*			monitor = Monitor::getMonitorInstance();
		ByteStream*			stream;

		int					i;
		unsigned long		color_type = 0;

		part_qos = new DomainParticipantQos();
		part_qos->entity_factory.auto_enable_created_entities = true;

		part = tnsDomainPart;
		part = new DomainParticipant(domain_id, 1);
		part->set_qos(part_qos);

		printf("DomainParticipant is created\n");

		topic = new Topic("Square", "ShapeType");
		topic->related_participant = part;

		printf("Topic is created");


		pub_qos = new PublisherQos();
		part->get_default_publisher_qos(pub_qos);

		pub_qos->entity_factory.auto_enable_created_entities = true;
		pub_qos->presentation.access_scope = INSTANCE_PRESENTATION_QOS;
		pub_qos->presentation.coherent_access = false;
		pub_qos->presentation.ordered_access = false;

		pub = part->create_publisher(pub_qos, NULL, 0x00000000);
		pub->related_participant = part;
		delete(pub_qos);

		printf("Publisher is created\n");

		i = 1;
		while (i != 0)
		{
			printf("\n");
			printf("[1] create a datawriter\n");
			printf("[0] exit\n");
			printf("Input>");
			scanf("%d", &i);

			writer_qos = new DataWriterQos();
			pub->get_default_datawriter_qos(writer_qos);

			writer_qos->reliability.kind = RELIABLE_RELIABILITY_QOS;
			writer_qos->ownership_strength.value = 0;
			writer_qos->liveliness.kind = AUTOMATIC_LIVELINESS_QOS;
			writer_qos->liveliness.lease_duration.sec = 0x7FFFFFFF;
			writer_qos->liveliness.lease_duration.nanosec = 0xFFFFFFFF;
			writer_qos->durabiltiy.kind = VOLATILE_DURABILITY_QOS;
			writer_qos->durability_service.history_kind = KEEP_LAST_HISTORY_QOS;
			writer_qos->durability_service.history_depth = 1;
			writer_qos->ownership.kind = SHARED_OWNERSHIP_QOS;
			writer_qos->destination_order.kind = BY_RECEPTION_TIMESTAMP_DESTINATIONORDER_QOS;
			writer_qos->deadline.period.sec = 0x7FFFFFFF;
			writer_qos->deadline.period.nanosec = 0xFFFFFFFF;
			writer_qos->latency_budget.duration.sec = 0x0;
			writer_qos->latency_budget.duration.nanosec = 0x0;
			writer_qos->lifespan.duration.sec = 0x7FFFFFFF;
			writer_qos->lifespan.duration.nanosec = 0xFFFFFFFF;

			writer = pub->create_datawriter(topic, writer_qos, NULL, 0x00000000);
			delete(writer_qos);

			printf("DataWriter is/are created\n");

			create_thread(0, CNU_DDS::write_shapetype, (void*)writer);
		}
	}

	void StartTopicNamingService(int startOption) {
		//initalDomainForTNS();
		controller = new TNSNController(startOption);
	}
}

