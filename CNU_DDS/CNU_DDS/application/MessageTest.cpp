#include "MessageTest.h"
#include <iostream>
#include "../include/Utility/PlatformOrientedInterface.h"
#include "../include/Utility/SystemInformation.h"
#include "../include/Utility/Network/TransceiverManager.h"

namespace CNU_DDS
{
	void	MessageTest()
	{
		Message*	send_msg	= new Message();
		Message*	recv_msg	= new Message();

		AckNack			acknack;
		Data			data;
		InfoDestination	info;

		unsigned char	sending_data[5]	= "data";
		ByteStream*		stream;

		send_msg->header.protocol	= PROTOCOL_RTPS;
		send_msg->header.version	= PROTOCOL_VERSION;
		send_msg->header.guid_prefix.host_id[0]	= 0x01;
		send_msg->header.guid_prefix.host_id[1]	= 0x02;
		send_msg->header.guid_prefix.host_id[2]	= 0x03;
		send_msg->header.guid_prefix.host_id[3]	= 0x04;
		send_msg->header.guid_prefix.app_id[0]	= 0x05;
		send_msg->header.guid_prefix.app_id[1]	= 0x06;
		send_msg->header.guid_prefix.app_id[2]	= 0x07;
		send_msg->header.guid_prefix.app_id[3]	= 0x08;
		send_msg->header.guid_prefix.counter[0]	= 0x09;
		send_msg->header.guid_prefix.counter[1]	= 0x0a;
		send_msg->header.guid_prefix.counter[2]	= 0x0b;
		send_msg->header.guid_prefix.counter[3]	= 0x0c;

		SequenceNumber_t*	seq_num_base	= new SequenceNumber_t();
		SequenceNumber_t*	seq_num_12		= new SequenceNumber_t();
		SequenceNumber_t*	seq_num_15		= new SequenceNumber_t();

		acknack.submessage_header.flags			= 0x01;
		acknack.submessage_header.submessage_id	= SUBMESSAGE_KIND_ACKNACK;

		acknack.reader_id.value.entity_key[0]	= 0;
		acknack.reader_id.value.entity_key[1]	= 1;
		acknack.reader_id.value.entity_key[2]	= 2;
		acknack.reader_id.value.entity_kind		= 3;

		acknack.writer_id.value.entity_key[0]	= 4;
		acknack.writer_id.value.entity_key[1]	= 5;
		acknack.writer_id.value.entity_key[2]	= 6;
		acknack.writer_id.value.entity_kind		= 7;

		seq_num_base->high	= 0;
		seq_num_base->low	= 10;
		seq_num_12->high	= 0;
		seq_num_12->low		= 12;
		seq_num_15->high	= 0;
		seq_num_15->low		= 15;

		acknack.reader_sn_state.base		= *seq_num_base;
		acknack.reader_sn_state.numBits		= 10;
		acknack.reader_sn_state.set.insertInRear(seq_num_base);
		acknack.reader_sn_state.set.insertInRear(seq_num_12);
		acknack.reader_sn_state.set.insertInRear(seq_num_15);

		acknack.count.value.value		= 30;

		data.submessage_header.flags			= 0x04 + 0x01;
		data.submessage_header.submessage_id	= SUBMESSAGE_KIND_DATA;

		data.reader_id.value.entity_key[0]	= 0;
		data.reader_id.value.entity_key[1]	= 1;
		data.reader_id.value.entity_key[2]	= 2;
		data.reader_id.value.entity_kind	= 3;

		data.writer_id.value.entity_key[0]	= 4;
		data.writer_id.value.entity_key[1]	= 5;
		data.writer_id.value.entity_key[2]	= 6;
		data.writer_id.value.entity_kind	= 7;

		data.writer_sn.value.high			= 200;
		data.writer_sn.value.low			= 400;

		data.serialized_payload.info[0]	= 0x00;
		data.serialized_payload.info[1]	= 0x01;
		data.serialized_payload.info[2]	= 0x00;
		data.serialized_payload.info[3]	= 0x00;
		data.serialized_payload.value.setData(SystemInformation::getSystemInformationInstance()->getSystemByteOrderingType(), sending_data, 5);

		info.guid_prefix.value.host_id[0]	= 0x01;
		info.guid_prefix.value.host_id[1]	= 0x02;
		info.guid_prefix.value.host_id[2]	= 0x03;
		info.guid_prefix.value.host_id[3]	= 0x04;
		info.guid_prefix.value.app_id[0]	= 0x05;
		info.guid_prefix.value.app_id[1]	= 0x06;
		info.guid_prefix.value.app_id[2]	= 0x07;
		info.guid_prefix.value.app_id[3]	= 0x08;
		info.guid_prefix.value.counter[0]	= 0x09;
		info.guid_prefix.value.counter[1]	= 0x0a;
		info.guid_prefix.value.counter[2]	= 0x0b;
		info.guid_prefix.value.counter[3]	= 0x0c;

		info.submessage_header.flags			= 0x01;
		info.submessage_header.submessage_id	= SUBMESSAGE_KIND_INFO_DESTINATION;

		send_msg->submessages->insertInRear(&acknack);
		send_msg->submessages->insertInRear(&data);
		send_msg->submessages->insertInRear(((Submessage*)&info));

		stream	= send_msg->Serialize();

		recv_msg->Deserialize(stream);
	}

	void	SubmessageTest()
	{
		std::cout<<"\tTest result : " << SubmessageTest_AckNack() << std::endl;
		std::cout<<"\tTest result : " << SubmessageTest_Data() << std::endl;
		std::cout<<"\tTest result : " << SubmessageTest_DataFrag() << std::endl;
		std::cout<<"\tTest result : " << SubmessageTest_Gap() << std::endl;
		std::cout<<"\tTest result : " << SubmessageTest_Heartbeat() << std::endl;
		std::cout<<"\tTest result : " << SubmessageTest_HeartbeatFrag() << std::endl;
		std::cout<<"\tTest result : " << SubmessageTest_InfoDestination() << std::endl;
		std::cout<<"\tTest result : " << SubmessageTest_InfoReply() << std::endl;
		std::cout<<"\tTest result : " << SubmessageTest_InfoSource() << std::endl;
		std::cout<<"\tTest result : " << SubmessageTest_InfoTimestamp() << std::endl;
		std::cout<<"\tTest result : " << SubmessageTest_NackFrag() << std::endl;
	}

	void	TransceiverTest()
	{
		TransceiverManager*	tm			= TransceiverManager::getManagerInstance();
		Transceiver*		t_1			= NULL;
		Endpoint			e_1;
		Transceiver*		t_2			= NULL;
		Locator_t			locator_1	= {LOCATOR_KIND_UDPv4, 7400, "168.188.128.147"};
		Locator_t			locator_2	= {LOCATOR_KIND_UDPv4, 7600, "168.188.128.147"};
		ByteStream*			stream		= NULL;
		AckNack				old_an;
		Message*			send_msg	= new Message();

		std::cout << "Submessage Test [AckNack]" << std::endl;

		old_an.submessage_header.flags			= 0x01;
		old_an.submessage_header.submessage_id	= SUBMESSAGE_KIND_ACKNACK;
		old_an.reader_id.value = ENTITYID_SEDP_BUILTIN_PUBLICATIONS_READER;
		old_an.writer_id.value = ENTITYID_SEDP_BUILTIN_PUBLICATIONS_WRITER;
		SequenceNumber_t*	seq_num_base	= new SequenceNumber_t();
		SequenceNumber_t*	seq_num_12		= new SequenceNumber_t();
		SequenceNumber_t*	seq_num_15		= new SequenceNumber_t();
		seq_num_base->high	= 0;
		seq_num_base->low	= 10;
		seq_num_12->high	= 0;
		seq_num_12->low		= 12;
		seq_num_15->high	= 0;
		seq_num_15->low		= 15;
		old_an.reader_sn_state.base		= *seq_num_base;
		old_an.reader_sn_state.numBits	= 10;
		old_an.reader_sn_state.set.insertInRear(seq_num_base);
		old_an.reader_sn_state.set.insertInRear(seq_num_12);
		old_an.reader_sn_state.set.insertInRear(seq_num_15);
		old_an.count.value.value		= 30;

		send_msg->header.protocol	= PROTOCOL_RTPS;
		send_msg->header.version	= PROTOCOL_VERSION;
		send_msg->header.guid_prefix.host_id[0]	= 0x01;
		send_msg->header.guid_prefix.host_id[1]	= 0x02;
		send_msg->header.guid_prefix.host_id[2]	= 0x03;
		send_msg->header.guid_prefix.host_id[3]	= 0x04;
		send_msg->header.guid_prefix.app_id[0]	= 0x05;
		send_msg->header.guid_prefix.app_id[1]	= 0x06;
		send_msg->header.guid_prefix.app_id[2]	= 0x07;
		send_msg->header.guid_prefix.app_id[3]	= 0x08;
		send_msg->header.guid_prefix.counter[0]	= 0x09;
		send_msg->header.guid_prefix.counter[1]	= 0x0a;
		send_msg->header.guid_prefix.counter[2]	= 0x0b;
		send_msg->header.guid_prefix.counter[3]	= 0x0c;

		send_msg->submessages->insertInRear(&old_an);
		stream	= send_msg->Serialize();

		std::cout<<"\tTransceiver Test begin\n";

		t_1	= tm->registerTransceiver(locator_1, 1234);
		t_2	= tm->registerTransceiver(locator_2, 5678);

		e_1.guid.entity_id	= ENTITYID_SEDP_BUILTIN_PUBLICATIONS_WRITER;
		t_1->regist(&e_1);

		std::cout<<"\tBefore sending \n";
		native_sleep(1000);

		t_2->send(stream, locator_1);

		native_sleep(3000);
		std::cout<<"\tAfter sending \n";
	}

	void	MultiMessagePerformanceTest()
	{
		Message*	send_msg	= new Message();
		Message*	recv_msgs[1000];
		int			i;
		
		for(i=0; i<1000; i++)
		{
			recv_msgs[i]	= new Message();
		}

		Data*				data;
		InfoDestination*	info;

		unsigned char	sending_data[5]	= "data";
		ByteStream*		stream;

		LARGE_INTEGER	time_begin;
		LARGE_INTEGER	time_end;
		INT64			time_calc;
		double			result;

		LARGE_INTEGER	freq;
		QueryPerformanceFrequency(&freq);

		send_msg->header.protocol	= PROTOCOL_RTPS;
		send_msg->header.version	= PROTOCOL_VERSION;
		send_msg->header.guid_prefix.host_id[0]	= 0x01;
		send_msg->header.guid_prefix.host_id[1]	= 0x02;
		send_msg->header.guid_prefix.host_id[2]	= 0x03;
		send_msg->header.guid_prefix.host_id[3]	= 0x04;
		send_msg->header.guid_prefix.app_id[0]	= 0x05;
		send_msg->header.guid_prefix.app_id[1]	= 0x06;
		send_msg->header.guid_prefix.app_id[2]	= 0x07;
		send_msg->header.guid_prefix.app_id[3]	= 0x08;
		send_msg->header.guid_prefix.counter[0]	= 0x09;
		send_msg->header.guid_prefix.counter[1]	= 0x0a;
		send_msg->header.guid_prefix.counter[2]	= 0x0b;
		send_msg->header.guid_prefix.counter[3]	= 0x0c;

		for(i=0; i<1; i++)
		{
			data	= new Data();

			data->submessage_header.flags			= 0x04 + 0x01;
			data->submessage_header.submessage_id	= SUBMESSAGE_KIND_DATA;

			data->reader_id.value.entity_key[0]	= 0;
			data->reader_id.value.entity_key[1]	= 1;
			data->reader_id.value.entity_key[2]	= 2;
			data->reader_id.value.entity_kind	= 3;

			data->writer_id.value.entity_key[0]	= 4;
			data->writer_id.value.entity_key[1]	= 5;
			data->writer_id.value.entity_key[2]	= 6;
			data->writer_id.value.entity_kind	= 7;

			data->writer_sn.value.high			= 0;
			data->writer_sn.value.low			= i;

			data->serialized_payload.info[0]	= 0x00;
			data->serialized_payload.info[1]	= 0x01;
			data->serialized_payload.info[2]	= 0x00;
			data->serialized_payload.info[3]	= 0x00;
			data->serialized_payload.value.setData(SystemInformation::getSystemInformationInstance()->getSystemByteOrderingType(), sending_data, 5);

			send_msg->submessages->insertInRear(data);
		}

		for(i=1; i<1; i++)
		{
			info	= new InfoDestination();

			info->guid_prefix.value.host_id[0]	= 0x01;
			info->guid_prefix.value.host_id[1]	= 0x02;
			info->guid_prefix.value.host_id[2]	= 0x03;
			info->guid_prefix.value.host_id[3]	= 0x04;
			info->guid_prefix.value.app_id[0]	= 0x05;
			info->guid_prefix.value.app_id[1]	= 0x06;	
			info->guid_prefix.value.app_id[2]	= 0x07;
			info->guid_prefix.value.app_id[3]	= 0x08;
			info->guid_prefix.value.counter[0]	= 0x09;
			info->guid_prefix.value.counter[1]	= 0x0a;
			info->guid_prefix.value.counter[2]	= 0x0b;
			info->guid_prefix.value.counter[3]	= 0x0c;

			info->submessage_header.flags			= 0x01;
			info->submessage_header.submessage_id	= SUBMESSAGE_KIND_INFO_DESTINATION;

			send_msg->submessages->insertInRear(info);
		}

		stream	= send_msg->Serialize();

		Transceiver*	tr	= new Transceiver();
		for(int j=0; j<5; j++)
		{
			QueryPerformanceCounter(&time_begin);
	
			for(i=0; i<1000; i++)
			{
				recv_msgs[i]->Deserialize(stream);
				//tr->message_decomposition(recv_msgs[i]);
			}
	
			QueryPerformanceCounter(&time_end);

			time_calc	= time_end.QuadPart	- time_begin.QuadPart;
			result		= (double)time_calc/(double)freq.QuadPart;
			printf("\tDESERIALIZATION TIME: %lf\n", result);
		}
	}

	bool	SubmessageTest_AckNack()
	{
		bool		result	= true;
		AckNack		old_an;
		AckNack		new_an;
		ByteStream*	stream;

		std::cout << "Submessage Test [AckNack]" << std::endl;

		old_an.submessage_header.flags			= 0x01;
		old_an.submessage_header.submessage_id	= SUBMESSAGE_KIND_ACKNACK;


		old_an.reader_id.value.entity_key[0]	= 0;
		old_an.reader_id.value.entity_key[1]	= 1;
		old_an.reader_id.value.entity_key[2]	= 2;
		old_an.reader_id.value.entity_kind		= 3;

		old_an.writer_id.value.entity_key[0]	= 4;
		old_an.writer_id.value.entity_key[1]	= 5;
		old_an.writer_id.value.entity_key[2]	= 6;
		old_an.writer_id.value.entity_kind		= 7;

		SequenceNumber_t*	seq_num_base	= new SequenceNumber_t();
		SequenceNumber_t*	seq_num_12		= new SequenceNumber_t();
		SequenceNumber_t*	seq_num_15		= new SequenceNumber_t();

		seq_num_base->high	= 0;
		seq_num_base->low	= 10;
		seq_num_12->high	= 0;
		seq_num_12->low		= 12;
		seq_num_15->high	= 0;
		seq_num_15->low		= 15;

		old_an.reader_sn_state.base		= *seq_num_base;
		old_an.reader_sn_state.numBits	= 10;
		old_an.reader_sn_state.set.insertInRear(seq_num_base);
		old_an.reader_sn_state.set.insertInRear(seq_num_12);
		old_an.reader_sn_state.set.insertInRear(seq_num_15);

		old_an.count.value.value		= 30;

		stream	= old_an.Serialize();
		new_an.submessage_header.flags				= 0x01;
		new_an.submessage_header.submessage_id		= SUBMESSAGE_KIND_ACKNACK;
		new_an.submessage_header.submessage_length	= old_an.submessage_header.submessage_length;
		new_an.Deserialize(stream->getData());
		delete(stream);

		if(memcmp(&old_an.reader_id.value, &new_an.reader_id.value, sizeof(EntityId_t)))
		{
			return false;
		}
		if(memcmp(&old_an.writer_id.value, &new_an.writer_id.value, sizeof(EntityId_t)))
		{
			return false;
		}
		if(old_an.reader_sn_state.numBits != new_an.reader_sn_state.numBits)
		{
			return false;
		}
		if(new_an.reader_sn_state.base.high != 0 || new_an.reader_sn_state.base.low != 10)
		{
			return false;
		}
		if(new_an.reader_sn_state.set.getNodeByIndex(1)->value->high != 0 || new_an.reader_sn_state.set.getNodeByIndex(1)->value->low != 10)
		{
			return false;
		}
		if(new_an.reader_sn_state.set.getNodeByIndex(2)->value->high != 0 || new_an.reader_sn_state.set.getNodeByIndex(2)->value->low != 12)
		{
			return false;
		}
		if(new_an.reader_sn_state.set.getNodeByIndex(3)->value->high != 0 || new_an.reader_sn_state.set.getNodeByIndex(3)->value->low != 15)
		{
			return false;
		}
		if(old_an.count.value.value != new_an.count.value.value)
		{
			return false;
		}

		return result;
	}

	bool	SubmessageTest_Data()
	{
		bool		result	= true;
		Data		old_data;
		Data		new_data;
		ByteStream	stream;

		std::cout << "Submessage Test [Data]" << std::endl;

		return result;
	}

	bool	SubmessageTest_DataFrag()
	{
		bool		result	= true;
		DataFrag	old_df;
		DataFrag	new_df;
		ByteStream	stream;

		std::cout << "Submessage Test [DataFrag]" << std::endl;

		return result;
	}

	bool	SubmessageTest_Gap()
	{
		bool		result	= true;
		Gap			old_gap;
		Gap			new_gap;
		ByteStream	stream;

		std::cout << "Submessage Test [Gap]" << std::endl;

		return result;
	}

	bool	SubmessageTest_Heartbeat()
	{
		bool		result	= true;
		Heartbeat	old_hb;
		Heartbeat	new_hb;
		ByteStream	stream;

		std::cout << "Submessage Test [Heartbeat]" << std::endl;

		return result;
	}

	bool	SubmessageTest_HeartbeatFrag()
	{
		bool			result	= true;
		HeartbeatFrag	old_hbf;
		HeartbeatFrag	new_hbf;
		ByteStream		stream;

		std::cout << "Submessage Test [HeartbeatFrag]" << std::endl;

		return result;
	}

	bool	SubmessageTest_InfoDestination()
	{
		bool			result	= true;
		InfoDestination	old_id;
		InfoDestination	new_id;
		ByteStream		stream;

		std::cout << "Submessage Test [InfoDestination]" << std::endl;

		return result;
	}

	bool	SubmessageTest_InfoReply()
	{
		bool		result	= true;
		InfoReply	old_ir;
		InfoReply	new_ir;
		ByteStream	stream;

		std::cout << "Submessage Test [InfoReply]" << std::endl;

		return result;
	}

	bool	SubmessageTest_InfoSource()
	{
		bool		result	= true;
		InfoSource	old_is;
		InfoSource	new_is;
		ByteStream	stream;

		std::cout << "Submessage Test [InfoSource]" << std::endl;

		return result;
	}

	bool	SubmessageTest_InfoTimestamp()
	{
		bool			result	= true;
		InfoTimestamp	old_it;
		InfoTimestamp	new_it;
		ByteStream		stream;

		std::cout << "Submessage Test [InfoTimestamp]" << std::endl;

		return result;
	}

	bool	SubmessageTest_NackFrag()
	{
		bool		result	= true;
		NackFrag	old_nf;
		NackFrag	new_nf;
		ByteStream	stream;

		std::cout << "Submessage Test [NackFrag]" << std::endl;

		return result;
	}
}
