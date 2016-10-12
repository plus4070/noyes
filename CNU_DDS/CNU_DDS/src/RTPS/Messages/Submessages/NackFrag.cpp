#include "../../../../include/RTPS/Messages/Submessages/NackFrag.h"
#include "../../../../include/Utility/SystemInformation.h"

namespace CNU_DDS
{
	NackFrag::NackFrag()
	{

	}

	NackFrag::~NackFrag()
	{

	}

	ByteStream*	NackFrag::Serialize()
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

		//writerSN
		memcpy(tmp_data + data_index, &this->writer_sn.value, sizeof(SequenceNumber_t));
		data_index	+= sizeof(SequenceNumber_t);

		//fragmentNumberState
		//bitmapBase
		memcpy(tmp_data + data_index, &this->fragment_number_state.base, sizeof(FragmentNumber_t));
		data_index	+= sizeof(FragmentNumber_t);

		//numBits
		memcpy(tmp_data + data_index, &this->fragment_number_state.numBits, sizeof(unsigned long));
		data_index	+= sizeof(unsigned long);

		//bitmap
		while(this->fragment_number_state.set.getSize() > 0)
		{
			FragmentNumber_t*	cur_frag	= this->fragment_number_state.set.popFirst();
			FragmentNumber_t	calc_frag;
			calc_frag.value	= cur_frag->value - this->fragment_number_state.base.value;

			if(calc_frag.value < 256)
			{
				tmp_data[data_index + calc_frag.value/8] |= (0x80 >> (calc_frag.value%8));
			}
			else
			{
				// do nothing
			}

			delete(cur_frag);
		}
		data_index	+= ((this->fragment_number_state.numBits+31)/32) * 4/*the size of one bitmap block*/;


		//count
		memcpy(tmp_data + data_index, &this->count.value, sizeof(Count_t));
		data_index	+= sizeof(Count_t);

		/*
		 * SubmessageHeader part
		 */
		this->submessage_header.submessage_id		= SUBMESSAGE_KIND_NACK_FRAG;
		this->submessage_header.submessage_length	= data_index - sizeof(SubmessageHeader);

		if (si->getSystemByteOrderingType() == BYTE_ORDERING_TYPE_LITTLE_ENDIAN)
		{
			this->submessage_header.flags |= 0x01;
		}

		memcpy(tmp_data, &this->submessage_header, sizeof(SubmessageHeader));
		new_stream->setData(si->getSystemByteOrderingType(), tmp_data, data_index);
		return new_stream;
	}

	void		NackFrag::Deserialize(unsigned char*	data)
	{
		SystemInformation*	si 			= SystemInformation::getSystemInformationInstance();
		unsigned char		tmp_char	= 0;
		unsigned long		i			= 0;
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

			//(FragmentNumber_t) fragmentNumberState.base
			memcpy(&this->fragment_number_state.base.value, recv_data + data_index, sizeof(FragmentNumber_t));
			data_index	+= sizeof(FragmentNumber_t);

			//(unsigned long) fragmentNumberState.numBits
			memcpy(&this->fragment_number_state.numBits, recv_data + data_index, sizeof(long));
			data_index	+= sizeof(long);

			//(bitmap) fragmentNumberState.bitmap
			for(i=0; i<this->fragment_number_state.numBits; i++)
			{
				tmp_char	= recv_data[data_index + i/8];

				if( (tmp_char & (0x80 >> i%8)) != 0 )
				{
					FragmentNumber_t*	recv_fn	= new FragmentNumber_t();
					FragmentNumber_t	cur_fn	= {i};

					*recv_fn	= this->fragment_number_state.base + cur_fn;

					this->fragment_number_state.set.insertInRear(recv_fn);
				}
			}
			data_index	+= ((this->fragment_number_state.numBits+31)/32) * 4/*the size of one bitmap block*/;

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

			//(FragmentNumber_t) fragmentNumberState.base
			memcpy(&this->fragment_number_state.base.value, recv_data + data_index, sizeof(FragmentNumber_t));
			data_index	+= sizeof(FragmentNumber_t);
			ByteStream::ChangeByteOrdering(&this->fragment_number_state.base.value, sizeof(long));

			//(unsigned long) fragmentNumberState.numBits
			memcpy(&this->fragment_number_state.numBits, recv_data + data_index, sizeof(long));
			data_index	+= sizeof(long);
			ByteStream::ChangeByteOrdering(&this->fragment_number_state.numBits, sizeof(long));

			//(bitmap) fragmentNumberState.bitmap
			for(i=0; i<this->fragment_number_state.numBits; i++)
			{
				tmp_char	= recv_data[data_index + i/8];

				if( (tmp_char & (0x80 >> i%8)) != 0 )
				{
					FragmentNumber_t*	recv_fn	= new FragmentNumber_t();
					FragmentNumber_t	cur_fn	= {i};

					*recv_fn	= this->fragment_number_state.base + cur_fn;

					this->fragment_number_state.set.insertInRear(recv_fn);
				}
			}
			data_index	+= ((this->fragment_number_state.numBits+31)/32) * 4/*the size of one bitmap block*/;

			//(Count_t) count
			memcpy(&this->count.value, recv_data + data_index, sizeof(Count_t));
			data_index	+= sizeof(Count_t);
			ByteStream::ChangeByteOrdering(&this->count.value.value, sizeof(long));
		}
	}
}
