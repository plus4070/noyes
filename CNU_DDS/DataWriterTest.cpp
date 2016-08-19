#include "include/DCPS/Domain/DomainParticipant.h"
#include "include/DCPS/Publication/Publisher.h"
#include "include/DCPS/Publication/DataWriter.h"
#include "include/DCPS/Subscription/Subscriber.h"
#include "include/DCPS/Subscription/DataReader.h"
#include "include/DCPS/Topic/Topic.h"
#include "include/RTPS/Behavior/StatefulWriter.h"
#include "include/RTPS/Behavior/StatefulReader.h"
#include "include/Utility/PlatformOrientedInterface.h"
#include "include/Utility/SystemInformation.h"

#include "application/DCPS/ThroughputListener.h"

#include <stdio.h>

namespace CNU_DDS
{
	void	settingDefaultParametersForDataWriter();
	void	StreamingTest_W(unsigned long domain_id, unsigned long	num_of_endpoints, unsigned long topic_mode, unsigned long topic_entry, unsigned long num_of_data, unsigned long data_sending_delay);

	void dataWriterTest(){
		printf("[ Data Writer Testing ..... ]\n\n");
		settingDefaultParametersForDataWriter();
	}

	void settingDefaultParametersForDataWriter(){
		unsigned long	domain_id;
		unsigned long	endpoint_type;
		unsigned long	num_of_endpoints;
		unsigned long	topic_mode;
		unsigned long	topic_number;
		unsigned long	num_of_data;
		unsigned long	data_sending_delay;
		
		printf("Input Domain ID>");
		scanf("%d", &domain_id);

		
		// ÀÔ·Â °Ç³Ê¶Ü
		fflush(stdin);
		
		endpoint_type = 2;


		// END Point °³¼ö = TopicÀÇ °¹¼ö = WriterÀÇ °¹¼ö
		switch(endpoint_type)
		{
		case 1:
		case 2:
			printf("\n");
			printf("Input the number of %s >", endpoint_type==1?"DataWriters":"DataReaders");
			scanf("%d", &num_of_endpoints);
			break;
		default:
			return;
		}

		printf("\n");
		printf("***** Select topic matching type *****\n");
		printf("[1] Same topic mode\n");
		printf("[2] Different topic mode\n");
		printf("[others] \n");
		printf("**************************************\n");
		printf("input>");
		scanf("%d", &topic_mode);

		switch(topic_mode)
		{
		case 1:
			topic_number	= 0;
			break;
		case 2:
			printf("input topic entry number>");
			scanf("%d", &topic_number);
			break;
		default:
			return;
		}

		printf("\n");
		printf("Input the number of data>");
		scanf("%d", &num_of_data);
		printf("Input data sending delay>");
		scanf("%d", &data_sending_delay);

		if((num_of_data <= 0) || (data_sending_delay < 0))
		{
			return;
		}

		StreamingTest_W(domain_id, num_of_endpoints, topic_mode, topic_number, num_of_data, data_sending_delay);
	}

	void	StreamingTest_W(unsigned long domain_id, unsigned long	num_of_endpoints, unsigned long topic_mode, unsigned long topic_entry, unsigned long num_of_data, unsigned long data_sending_delay)
	{
		/* Declaration */
		DomainParticipantQos*	part_qos;
		DomainParticipant*		part;
		Topic**					topics;

		PublisherQos*		pub_qos;
		Publisher*			pub;
		DataWriterQos*		writer_qos;
		DataWriter**		writers;

		int	check;
		int num_of_topics;
		int	i, j;

		unsigned long	time_start;
		unsigned long	time_end;

		printf("\n");
		printf("********** Streaming test environment **********\n");
		printf("Domain ID        : %d\n", domain_id);
		printf("Endpoint type    : DataWriter\n");
		printf("Num of endpoints : %d\n", num_of_endpoints);
		printf("Matching type    : %s\n", topic_mode==1?"Same":"Different");
		printf("Num of data      : %d\n", num_of_data);
		printf("Sending delay    : %d\n", data_sending_delay);
		printf("************************************************\n");
		printf("Check test environment(Yes: 1 / No: others)>");
		scanf("%d", &check);
		printf("\n");

		if(check != 1)
		{
			return;
		}
		printf("\n");

/* File */
		sprintf(chr_file_name, "[CNU DDS %08X] Writer N(%d),M(%s),C(%d),D(%d).csv", (unsigned long)get_system_time(), num_of_endpoints, topic_mode==1?"Same":"Diff", num_of_data, data_sending_delay);
		file_pointer	= fopen(chr_file_name, "w+");
		fprintf(file_pointer, "Start,End,Differential\n");
		fflush(file_pointer);

		if(file_pointer == 0)
		{
			printf("ERROR: A FILE OPEN FUNCTION IS FAILED\n");
			exit(0);
		}
		else
		{
			printf("File %s is created\n", chr_file_name);
		}

/* DomainParticipant */
		part_qos												= new DomainParticipantQos();
		part_qos->entity_factory.auto_enable_created_entities	= true;

		part													= new DomainParticipant(domain_id, 1);
		part->set_qos(part_qos);

		printf("DomainParticipant is created\n");

/* Topic */
		if(topic_mode == 1)
		{
			num_of_topics	= 1;
		}
		else
		{
			num_of_topics	= num_of_endpoints;
		}
		topics	= (Topic**)malloc(sizeof(Topic*) * num_of_topics);

		for(i=0; i<num_of_topics; i++)
		{
			char	chr_topic_name[32]	= {0, };
			char	chr_type_name[32]	= {0, };

			sprintf(chr_topic_name, "TopicName_%d",i+topic_entry);
			sprintf(chr_type_name, "TypeName_%d", i+topic_entry);

			std::string	str_topic_name	= chr_topic_name;
			std::string	str_type_name	= chr_type_name;

			topics[i]						= new Topic(str_topic_name, str_type_name);
			topics[i]->related_participant	= part;
		}

		printf("%d Topics is/are created\n", num_of_topics);

/* Publisher */
		pub_qos		= new PublisherQos();
		part->get_default_publisher_qos(pub_qos);
		pub			= part->create_publisher(pub_qos, NULL, 0x00000000);
		pub->related_participant	= part;
		delete(pub_qos);

		printf("Publisher is created\n");

/* DataWriter */
		writer_qos	= new DataWriterQos();
		pub->get_default_datawriter_qos(writer_qos);
		writers		= (DataWriter**)malloc(sizeof(DataWriter*) * num_of_endpoints);

		for(i=0; i<num_of_endpoints; i++)
		{
			writers[i]	= pub->create_datawriter(topic_mode==1?topics[0]:topics[i], writer_qos, NULL, 0x00000000);
		}
		DataWriterQos*	writer_qos_2	= new DataWriterQos();
		writers[0]->get_qos(writer_qos_2);
		delete(writer_qos);

		printf("%d DataWriter is/are created\n", num_of_endpoints);

/* Sending */
		printf("A data publishing is ready(wait 5 seconds)\n", num_of_endpoints);

		for(i=0; i<10; i++)
		{
			native_sleep(1000);
			printf(".");
			fflush(0);
		}
		printf("\n");

		ByteStream*			stream	= new ByteStream();
		unsigned char		data[5]	= "TEST";
		stream->setData(BYTE_ORDERING_TYPE_LITTLE_ENDIAN, data, 5);

		time_start	= get_current_time();

		for(i=0; i<num_of_data; i++)
		{
			for(j=0; j<num_of_endpoints; j++)
			{
				((StatefulWriter*)writers[j]->related_rtps_writer)->new_change(CHANGE_KIND_ALIVE, stream, 0);
			}
			native_sleep(data_sending_delay);
			//printf("If you want to send a data, input any number\n");
			//scanf("%d", &j);
			//printf("\tsend(%2d)\n", i);
		}
		time_end	= get_current_time();

		fprintf(file_pointer, "%d,%d,%d\n", time_start, time_end, time_end-time_start);
		fflush(file_pointer);
		fclose(file_pointer);

		printf("Sending(%d) is over\n", num_of_data);
		printf("If you want to terminate this program, put any number>");
		scanf("%d", &i);
	}
}