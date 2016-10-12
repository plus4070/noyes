#include "../../../../include/RTPS/Messages/Submessages/DataFrag.h"
#include "../../../../include/Utility/SystemInformation.h"

namespace CNU_DDS
{
	DataFrag::DataFrag()
	{

	}

	DataFrag::~DataFrag()
	{

	}

	ByteStream*	DataFrag::Serialize()
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

		//extraFlags
		unsigned short	tmp_short	= 0x0000;
		memcpy(tmp_data + data_index, &tmp_short, sizeof(short));
		data_index	+= sizeof(short);

		//octetsToInlineQos
		tmp_short	= 0x001C;
		memcpy(tmp_data + data_index, &tmp_short, sizeof(short));
		data_index	+= sizeof(short);

		//readerId
		memcpy(tmp_data + data_index, &this->reader_id.value, sizeof(EntityId_t));
		data_index	+= sizeof(EntityId_t);

		//writerId
		memcpy(tmp_data + data_index, &this->writer_id.value, sizeof(EntityId_t));
		data_index	+= sizeof(EntityId_t);

		//writerSN
		memcpy(tmp_data + data_index, &this->writer_sn.value, sizeof(SequenceNumber_t));
		data_index	+= sizeof(SequenceNumber_t);

		//fragmentStartingNum
		memcpy(tmp_data + data_index, &this->fragment_starting_num.value, sizeof(FragmentNumber_t));
		data_index	+= sizeof(FragmentNumber_t);

		//fragmentsInSubmessage
		memcpy(tmp_data + data_index, &this->fragments_in_submessage, sizeof(short));
		data_index	+= sizeof(short);

		//fragmentSize
		memcpy(tmp_data + data_index, &this->fragement_size, sizeof(short));
		data_index	+= sizeof(short);

		//sampleSize
		memcpy(tmp_data + data_index, &this->data_size, sizeof(long));
		data_index	+= sizeof(long);

		//inlineQos
		if(this->submessage_header.flags | 0x02/*InlineQosFlag*/)
		{

		}

		//serializedPayload
		if(this->submessage_header.flags | 0x04/*DataFlag*/)
		{
			memcpy(tmp_data + data_index, this->serialized_payload.info, 4); //Encapsulation Identifier(size 2) + options(size 2)
			data_index	+= 4;

			memcpy(tmp_data + data_index, this->serialized_payload.value.getData(), this->serialized_payload.value.getLength());
			data_index	+= this->serialized_payload.value.getLength();
		}

		/*
		 * SubmessageHeader part
		 */
		this->submessage_header.submessage_id		= SUBMESSAGE_KIND_DATA_FRAG;
		this->submessage_header.submessage_length	= data_index - sizeof(SubmessageHeader);

		if (si->getSystemByteOrderingType() == BYTE_ORDERING_TYPE_LITTLE_ENDIAN)
		{
			this->submessage_header.flags |= 0x01;
		}

		memcpy(tmp_data, &this->submessage_header, sizeof(SubmessageHeader));
		new_stream->setData(si->getSystemByteOrderingType(), tmp_data, data_index);
		return new_stream;
	}

	void		DataFrag::Deserialize(unsigned char*	data)
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

			//(2 bytes) extraFlags
			memcpy(this->extraFlags, recv_data + data_index, 2);
			data_index	+= 2;

			//(unsigned short) octetsToInlineQos - pass
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

			//(FragmentNumber_t) fragmentStartingNum
			memcpy(&this->fragment_starting_num.value, recv_data + data_index, sizeof(FragmentNumber_t));
			data_index	+= sizeof(FragmentNumber_t);

			//(unsigned short) fragmentsInSubmessage
			memcpy(&this->fragments_in_submessage, recv_data + data_index, sizeof(short));
			data_index	+= sizeof(short);

			//(unsigned short) fragmentSize
			memcpy(&this->fragement_size, recv_data + data_index, sizeof(short));
			data_index	+= sizeof(short);

			//(unsigned long) sampleSize
			memcpy(&this->data_size, recv_data + data_index, sizeof(long));
			data_index	+= sizeof(long);

			//(ParameterList_t) inlineQos ( only if Q-bit(0x02) )
			if(this->submessage_header.flags | 0x02/*InlineQosFlag*/ )
			{

			}

			//(SerializedPayload) serializedPayload ( only if D-bit(0x04) || K-bit(0x08) )
			if((this->submessage_header.flags | 0x04/*DataFlag*/) || (this->submessage_header.flags | 0x08/*KeyFlag*/))
			{
				memcpy(this->serialized_payload.info, recv_data + data_index, 4); //Encapsulation Identifier(size 2) + options(size 2)
				data_index	+= 4;

				if(this->serialized_payload.info[1] == 0x00 || this->serialized_payload.info[1] == 0x02)
				{
					this->serialized_payload.value.setData(BYTE_ORDERING_TYPE_BIG_ENDIAN, recv_data + data_index, submessage_header.submessage_length - data_index + sizeof(SubmessageHeader));
				}
				else
				{
					this->serialized_payload.value.setData(BYTE_ORDERING_TYPE_LITTLE_ENDIAN, recv_data + data_index, submessage_header.submessage_length - data_index + sizeof(SubmessageHeader));
				}
			}
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

			//(2 bytes) extraFlags
			memcpy(this->extraFlags, recv_data + data_index, 2);
			data_index	+= 2;

			//(unsigned short) octetsToInlineQos - pass
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
			ByteStream::ChangeByteOrdering(&this->writer_sn.value.high, sizeof(long));
			ByteStream::ChangeByteOrdering(&this->writer_sn.value.low, sizeof(long));

			//(FragmentNumber_t) fragmentStartingNum
			memcpy(&this->fragment_starting_num.value, recv_data + data_index, sizeof(FragmentNumber_t));
			data_index	+= sizeof(FragmentNumber_t);
			ByteStream::ChangeByteOrdering(&this->fragment_starting_num.value.value, sizeof(long));

			//(unsigned short) fragmentsInSubmessage
			memcpy(&this->fragments_in_submessage, recv_data + data_index, sizeof(short));
			data_index	+= sizeof(short);
			ByteStream::ChangeByteOrdering(&this->fragments_in_submessage, sizeof(short));

			//(unsigned short) fragmentSize
			memcpy(&this->fragement_size, recv_data + data_index, sizeof(short));
			data_index	+= sizeof(short);
			ByteStream::ChangeByteOrdering(&this->fragement_size, sizeof(short));

			//(unsigned long) sampleSize
			memcpy(&this->data_size, recv_data + data_index, sizeof(long));
			data_index	+= sizeof(long);
			ByteStream::ChangeByteOrdering(&this->data_size, sizeof(short));

			//(ParameterList_t) inlineQos ( only if Q-bit(0x02) )
			if(this->submessage_header.flags | 0x02/*InlineQosFlag*/ )
			{

			}

			//(SerializedPayload) serializedPayload ( only if D-bit(0x04) || K-bit(0x08) )
			if((this->submessage_header.flags | 0x04/*DataFlag*/) || (this->submessage_header.flags | 0x08/*KeyFlag*/))
			{
				memcpy(this->serialized_payload.info, recv_data + data_index, 4); //Encapsulation Identifier(size 2) + options(size 2)
				data_index	+= 4;

				if(this->serialized_payload.info[1] == 0x00 || this->serialized_payload.info[1] == 0x02)
				{
					this->serialized_payload.value.setData(BYTE_ORDERING_TYPE_BIG_ENDIAN, recv_data + data_index, submessage_header.submessage_length - data_index + sizeof(SubmessageHeader));
				}
				else
				{
					this->serialized_payload.value.setData(BYTE_ORDERING_TYPE_LITTLE_ENDIAN, recv_data + data_index, submessage_header.submessage_length - data_index + sizeof(SubmessageHeader));
				}
			}
		}
	}
}
