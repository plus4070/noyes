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

#include "../../include/RTPS/Discovery/SEDPdiscoveredPublicationData.h"
#include "../../include/RTPS/Discovery/SEDPdiscoveredSubscriptionData.h"
#include "../../include/Utility/Monitoring/Monitor.h"

#include "ShapeTest.h"
#include <stdio.h>

namespace CNU_DDS
{
unsigned long	color_type;

void*	write_shapetype(void* arg)
{
	DataWriter*	writer	= (DataWriter*)arg;
	ShapeType	st;
	char		buf[24]	= {0, };
	char		sending_status	= 0;
	
	st.lengthOfColor	= 5;
	st.x				= 50;
	st.y				= 50;
	st.shapesize		= 30;

	memcpy(buf, &st.lengthOfColor, 4);
	memcpy(buf + 4, "BLUE", 4);
	memcpy(buf + 20, &st.shapesize, 4);

	native_sleep(2000);

	while(true)
	{
		native_sleep(100);

		ByteStream*	data	= new ByteStream();

		switch(sending_status)
		{
		case 0:
			st.x++;
			if(st.x >= 200)
			{
				sending_status	= 1;
			}
			break;
		case 1:
			st.y++;
			if(st.y >= 200)
			{
				sending_status	= 2;
			}
			break;
		case 2:
			st.x--;
			if(st.x <= 50)
			{
				sending_status = 3;
			}
			break;
		case 3:
			st.y--;
			if(st.y <= 50)
			{
				sending_status	= 0;
			}
			break;
		}
		memcpy(buf + 12, &st.x, 4);
		memcpy(buf + 16, &st.y, 4);

		//printf("X(%3d), Y(%3d)\n", st.x, st.y);
		data->setData(BYTE_ORDERING_TYPE_LITTLE_ENDIAN, (unsigned char*)buf, 24);
		((StatefulWriter*)writer->related_rtps_writer)->new_change(CHANGE_KIND_ALIVE, data, 0);
	}
}

void	ShapeTest()
{
	printf("[ Shape test about CNU DDS ]\n\n");

	unsigned long	domain_id	= 0;
	
	DomainParticipantQos*	part_qos;
	DomainParticipant*		part;
	Topic*					topic;

	PublisherQos*		pub_qos;
	Publisher*			pub;
	DataWriterQos*		writer_qos;
	DataWriter*			writer;

	Monitor*			monitor	= Monitor::getMonitorInstance();
	ByteStream*			stream;

	int					i;
	color_type	= 0;
	
	part_qos												= new DomainParticipantQos();
	part_qos->entity_factory.auto_enable_created_entities	= true;

	part													= new DomainParticipant(domain_id, 1);
	part->set_qos(part_qos);

	printf("DomainParticipant is created\n");

	topic	= new Topic("Square", "ShapeType");
	topic->related_participant	= part;

	printf("Topic is created");

		
	pub_qos		= new PublisherQos();
	part->get_default_publisher_qos(pub_qos);

	pub_qos->entity_factory.auto_enable_created_entities	= true;
	pub_qos->presentation.access_scope	= INSTANCE_PRESENTATION_QOS;
	pub_qos->presentation.coherent_access	= false;
	pub_qos->presentation.ordered_access	= false;

	pub			= part->create_publisher(pub_qos, NULL, 0x00000000);
	pub->related_participant	= part;
	delete(pub_qos);

	printf("Publisher is created\n");

	i	= 1;
	while(i!=0)
	{

	printf("\n");
	printf("[1] create a datawriter\n");
	printf("[0] exit\n");
	printf("Input>");
	scanf("%d", &i);

	writer_qos	= new DataWriterQos();
	pub->get_default_datawriter_qos(writer_qos);

	writer_qos->reliability.kind					= RELIABLE_RELIABILITY_QOS;
	writer_qos->ownership_strength.value			= 0;
	writer_qos->liveliness.kind						= AUTOMATIC_LIVELINESS_QOS;
	writer_qos->liveliness.lease_duration.sec		= 0x7FFFFFFF;
	writer_qos->liveliness.lease_duration.nanosec	= 0xFFFFFFFF;
	writer_qos->durabiltiy.kind						= VOLATILE_DURABILITY_QOS;
	writer_qos->durability_service.history_kind		= KEEP_LAST_HISTORY_QOS;
	writer_qos->durability_service.history_depth	= 1;
	writer_qos->ownership.kind						= SHARED_OWNERSHIP_QOS;
	writer_qos->destination_order.kind				= BY_RECEPTION_TIMESTAMP_DESTINATIONORDER_QOS;
	writer_qos->deadline.period.sec					= 0x7FFFFFFF;
	writer_qos->deadline.period.nanosec				= 0xFFFFFFFF;
	writer_qos->latency_budget.duration.sec			= 0x0;
	writer_qos->latency_budget.duration.nanosec		= 0x0;
	writer_qos->lifespan.duration.sec				= 0x7FFFFFFF;
	writer_qos->lifespan.duration.nanosec			= 0xFFFFFFFF;

	writer	= pub->create_datawriter(topic, writer_qos, NULL, 0x00000000);
	delete(writer_qos);

	printf("DataWriter is/are created\n");

	create_thread(0, write_shapetype, (void*)writer);


	int								reader_counter		= 0;
	int								writer_counter		= 0;
	MonitoringData*					monitoring_data		= NULL;
	SEDPdiscoveredSubscriptionData	*sub_data			= new SEDPdiscoveredSubscriptionData();
	SEDPdiscoveredPublicationData	*pub_data			= new SEDPdiscoveredPublicationData();


	for(int i=0; i<100; i++)
	{
		native_sleep(100);

		/* Subscriber 정보 */
		while(monitoring_data = monitor->popReaderData())
		{
			
			sub_data->Deserialize(monitoring_data->data);
			sub_data->topic_name;
			sub_data->type_name;

			monitoring_data->locator->address;
			monitoring_data->locator->port;

			reader_counter++;
		}

		/* Publisher 정보 */
		while(monitoring_data = monitor->popWriterData())
		{
			pub_data->Deserialize(monitoring_data->data);
			pub_data->topic_name;
			pub_data->type_name;

			monitoring_data->locator->address;
			monitoring_data->locator->port;

			writer_counter++;
		}

		/* Subscriber 정보 삽입 */
		sub_data->Deserialize(monitoring_data->data);
		sub_data->unicast_locator_list->insertInRear(monitoring_data->locator);
		part->_remote_datareader_is_discovered(sub_data);
		
		/* Publisher 정보 삽입 */
		pub_data->Deserialize(monitoring_data->data);
		pub_data->unicast_locator_list->insertInRear(monitoring_data->locator);
		part->_remote_datawriter_is_discovered(pub_data);


		printf("[# of discovered reader] %d\n", reader_counter);
		printf("[# of discovered writer] %d\n", writer_counter);
		//reader_counter = 0;
		//writer_counter = 0;
	}

	}
	//printf("If you want to terminate this program, put any number>\n");
	//scanf("%d", &i);

	printf("\n[ EXIT ]\n");
}
}