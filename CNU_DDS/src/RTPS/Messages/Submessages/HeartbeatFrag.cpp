#include "../../../../include/RTPS/Messages/Submessages/HeartbeatFrag.h"
#include "../../../../include/Utility/SystemInformation.h"

namespace CNU_DDS
{
	HeartbeatFrag::HeartbeatFrag()
	{

	}

	HeartbeatFrag::~HeartbeatFrag()
	{

	}

	ByteStream*	HeartbeatFrag::Serialize()
	{
		ByteStream* 		new_stream 								= new ByteStream();
		unsigned char 		tmp_data[ByteStream::DATA_LENGTH_MAX]	= { 0, };
		SystemInformation* 	si 										= SystemInformation::getSystemInformationInstance();
		unsigned long 		data_index 								= 0;

		/*
		 * Submessage part
		 */

		//submessage header
		data_index = sizeof(SubmessageHeader);

		//readerId
		memcpy(tmp_data + data_index, &this->reader_id.value, sizeof(EntityId_t));
		data_index	+= sizeof(EntityId_t);

		//writerId
		memcpy(tmp_data + data_index, &this->writer_id.value, sizeof(EntityId_t));
		data_index	+= sizeof(EntityId_t);

		//firstSN
		memcpy(tmp_data + data_index, &this->writer_sn.value, sizeof(SequenceNumber_t));
		data_index	+= sizeof(SequenceNumber_t);

		//lastSN
		memcpy(tmp_data + data_index, &this->last_fragment_num.value, sizeof(FragmentNumber_t));
		data_index	+= sizeof(FragmentNumber_t);

		//count
		memcpy(tmp_data + data_index, &this->count.value, sizeof(Count_t));
		data_index	+= sizeof(Count_t);

		/*
		 * SubmessageHeader part
		 */
		this->submessage_header.submessage_id		= SUBMESSAGE_KIND_HEARTBEAT_FRAG;
		this->submessage_header.submessage_length	= data_index - sizeof(SubmessageHeader);

		if (si->getSystemByteOrderingType() == BYTE_ORDERING_TYPE_LITTLE_ENDIAN)
		{
			this->submessage_header.flags |= 0x01;
		}

		memcpy(tmp_data, &this->submessage_header, sizeof(SubmessageHeader));
		new_stream->setData(si->getSystemByteOrderingType(), tmp_data, data_index);
		return new_stream;
	}

	void		HeartbeatFrag::Deserialize(unsigned char*	data)
	{
		SystemInformation*	si 			= SystemInformation::getSystemInformationInstance();
		unsigned char*		recv_data	= data;
		unsigned long		data_index	= 0;
		ByteOrderingType	recv_ordering	= (recv_data[1] & 0x01)?BYTE_ORDERING_TYPE_LITTLE_ENDIAN:BYTE_ORDERING_TYPE_BIG_ENDIAN;

		if(si->getSystemByteOrderingType() == recv_ordering)
		{
			//Header
			//SubmessageKind
			this->submessage_header.submessage_id	= recv_data[data_index];
			data_index	+= 1;

			this->submessage_header.flags			= recv_data[data_index];
			data_index	+= 1;

			memcpy(&this->submessage_header.submessage_length, recv_data + data_index, sizeof(short));
			data_index	+= sizeof(short);

			//(EntityId_t) readerId
			memcpy(&this->reader_id.value, recv_data + data_index, sizeof(EntityId_t));
			data_index	+= sizeof(EntityId_t);

			//(EntityId_t) writerId
			memcpy(&this->writer_id.value, recv_data + data_index, sizeof(EntityId_t));
			data_index	+= sizeof(EntityId_t);

			//(SequenceNumber_t) writerSN
			memcpy(&this->writer_sn.value, recv_data + data_index, sizeof(SequenceNumber_t));
			data_index	+= sizeof(SequenceNumber_t);

			//(FragmentNumber_t) lastFragmentNum
			memcpy(&this->last_fragment_num.value, recv_data + data_index, sizeof(FragmentNumber_t));
			data_index	+= sizeof(FragmentNumber_t);

			//(Count_t) count
			memcpy(&this->count.value, recv_data + data_index, sizeof(Count_t));
			data_index	+= sizeof(Count_t);
		}
		else
		{
			//Header
			//SubmessageKind
			this->submessage_header.submessage_id	= recv_data[data_index];
			data_index	+= 1;

			this->submessage_header.flags			= recv_data[data_index];
			data_index	+= 1;

			memcpy(&this->submessage_header.submessage_length, recv_data + data_index, sizeof(short));
			data_index	+= sizeof(short);
			ByteStream::ChangeByteOrdering(&this->submessage_header.submessage_length, sizeof(short));

			//(EntityId_t) readerId
			memcpy(&this->reader_id.value, recv_data + data_index, sizeof(EntityId_t));
			data_index	+= sizeof(EntityId_t);

			//(EntityId_t) writerId
			memcpy(&this->writer_id.value, recv_data + data_index, sizeof(EntityId_t));
			data_index	+= sizeof(EntityId_t);

			//(SequenceNumber_t) writerSN
			memcpy(&this->writer_sn.value, recv_data + data_index, sizeof(SequenceNumber_t));
			data_index	+= sizeof(SequenceNumber_t);
			ByteStream::ChangeByteOrdering(&this->writer_sn.value.high, sizeof(long));
			ByteStream::ChangeByteOrdering(&this->writer_sn.value.low, sizeof(long));

			//(FragmentNumber_t) lastFragmentNum
			memcpy(&this->last_fragment_num.value, recv_data + data_index, sizeof(FragmentNumber_t));
			data_index	+= sizeof(FragmentNumber_t);
			ByteStream::ChangeByteOrdering(&this->last_fragment_num.value.value, sizeof(long));

			//(Count_t) count
			memcpy(&this->count.value, recv_data + data_index, sizeof(Count_t));
			data_index	+= sizeof(Count_t);
			ByteStream::ChangeByteOrdering(&this->count.value.value, sizeof(long));
		}
	}
}
