#include "DCPSTest.h"
#include "../../include/DCPS/Domain/DomainParticipant.h"
#include "../../include/DCPS/Publication/Publisher.h"
#include "../../include/DCPS/Publication/DataWriter.h"
#include "../../include/DCPS/Subscription/Subscriber.h"
#include "../../include/DCPS/Subscription/DataReader.h"
#include "../../include/DCPS/Topic/Topic.h"
#include "../../include/RTPS/Behavior/StatefulWriter.h"
#include "../../include/RTPS/Behavior/StatefulReader.h"
#include "../../include/Utility/PlatformOrientedInterface.h"
#include "../../include/Utility/SystemInformation.h"
#include "../../include/Utility/Network/Transceiver.h"
#include "TestListener.h"
#include <stdio.h>

namespace CNU_DDS
{
	void	DCPSTest()
	{

		int participant_id;
		int app_type;

		printf("DCPSTest begin!!\n");
		printf("input the domain ID: ");
		scanf("%d", &participant_id);

		printf("input application type\n");
		printf("[1] DataWriter\n");
		printf("[2] DataReader\n");
		printf("input> ");
		scanf("%d", &app_type);

		DomainParticipantQos*	part_qos	= new DomainParticipantQos();
		part_qos->entity_factory.auto_enable_created_entities	= true;
		printf("DomainParticipantQos is created\n");
		DomainParticipant*		part		= new DomainParticipant(participant_id, app_type);
		part->set_qos(part_qos);
		printf("DomainParticipant is created\n");

		Topic*				topic		= new Topic("TEST TOPIC!!!!", "TEST TYPE????");
		topic->related_participant		= part;

		PublisherQos*		pub_qos;
		Publisher*			pub;
		DataWriterQos*		writer_qos;
		DataWriter*			data_writer;
		SubscriberQos*		sub_qos;
		Subscriber*			sub;
		DataReaderQos*		reader_qos;
		DataReader*			data_reader;
		TestListener*		listener;
		ByteStream*			stream	= new ByteStream();
		unsigned char		data[5]	= "TEST";
		stream->setData(BYTE_ORDERING_TYPE_LITTLE_ENDIAN, data, 5);

		Transceiver*		transceiver;

		switch(app_type)
		{
		case 1:

			pub_qos		= new PublisherQos();
			printf("PublisherQos is created\n");
			pub			= part->create_publisher(pub_qos, NULL, 0x00000000);
			pub->related_participant	= part;
			printf("Publisher is created\n");

			writer_qos	= new DataWriterQos();
			printf("DataWriterQos is created\n");
			data_writer	= pub->create_datawriter(topic, writer_qos, NULL, 0x00000000);
			printf("DataWriter is created\n");

			native_sleep(5000);

			for(int i=0; i<READER_PROXY_CACHE_SIZE_MAX; i++)
			{
				((StatefulWriter*)data_writer->related_rtps_writer)->new_change(CHANGE_KIND_ALIVE, stream, 0);
				//printf("\tsend(%2d)\n", i);
			}

			printf("Sending(%d) is over\n", READER_PROXY_CACHE_SIZE_MAX);

			for(int j=0; j<5; j++)
			{
				native_sleep(1000);
			}

			break;

		case 2:

			sub_qos		= new SubscriberQos();
			printf("SubscriberQos is created\n");
			sub			= part->create_subscriber(sub_qos, NULL, 0x00000000);
			sub->related_participant	= part;
			printf("Subscriber is created\n");

			reader_qos	= new DataReaderQos();
			printf("DataReaderQos is created\n");
			listener	= new TestListener();
			data_reader	= sub->create_datareader(topic->get_topic_description(), reader_qos, listener, 0xFFFFFFFF);
			printf("DataReader is created\n");

			/*for(int i=0; i<30; i++)
			{
				native_sleep(1000);
			}*/
			printf("input any number\n");
			scanf("%d", &app_type);

			transceiver	= (Transceiver*)((StatefulReader*)data_reader->related_rtps_reader)->_get_transceiver();

			/*printf("Transceiver receiving count(%d)\n", transceiver->test_recv_count);
			printf("Transceiver processing count(%d)\n", transceiver->test_proc_count);
			printf("Receiving count(%d)\n", listener->count);*/

			break;

		default:
			break;
		}
	}
}
