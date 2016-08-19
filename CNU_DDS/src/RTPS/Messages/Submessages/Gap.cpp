#include "../../../../include/RTPS/Messages/Submessages/Gap.h"
#include "../../../../include/Utility/SystemInformation.h"

namespace CNU_DDS
{
	Gap::Gap()
	{

	}

	Gap::~Gap()
	{

	}

	ByteStream*	Gap::Serialize()
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

		//gapStart
		memcpy(tmp_data + data_index, &this->gap_start.value, sizeof(SequenceNumber_t));
		data_index	+= sizeof(SequenceNumber_t);

		//gapList.bitmapBase
		memcpy(tmp_data + data_index, &this->gap_list.base, sizeof(SequenceNumber_t));
		data_index	+= sizeof(SequenceNumber_t);

		//gapList.numBits
		memcpy(tmp_data + data_index, &this->gap_list.numBits, sizeof(long));
		data_index	+= sizeof(long);

		//gapList.bitmap
		while(this->gap_list.set.getSize() > 0)
		{
			SequenceNumber_t*	cur_seq		= this->gap_list.set.popFirst();
			SequenceNumber_t	calc_seq	= (*cur_seq) - this->gap_list.base;

			if(calc_seq.high == 0 && calc_seq.low < 256)
			{
				tmp_data[data_index + calc_seq.low/8] |= (0x80 >> (calc_seq.low%8));
			}
			else
			{
				// do nothing
			}

			delete(cur_seq);
		}
		data_index	+= ((this->gap_list.numBits+31)/32) * 4/*the size of one bitmap block*/;

		/*
		 * SubmessageHeader part
		 */
		this->submessage_header.submessage_id		= SUBMESSAGE_KIND_GAP;
		this->submessage_header.submessage_length	= data_index - sizeof(SubmessageHeader);

		if (si->getSystemByteOrderingType() == BYTE_ORDERING_TYPE_LITTLE_ENDIAN)
		{
			this->submessage_header.flags |= 0x01;
		}

		memcpy(tmp_data, &this->submessage_header, sizeof(SubmessageHeader));
		new_stream->setData(si->getSystemByteOrderingType(), tmp_data, data_index);
		return new_stream;
	}

	void		Gap::Deserialize(unsigned char*	data)
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

			//(SequenceNumber_t) gapStart
			memcpy(&this->gap_start.value, recv_data + data_index, sizeof(SequenceNumber_t));
			data_index	+= sizeof(SequenceNumber_t);

			//(SequenceNumber_t) gapList.base
			memcpy(&this->gap_list.base, recv_data + data_index, sizeof(SequenceNumber_t));
			data_index	+= sizeof(SequenceNumber_t);

			//(unsigned long) gapList.numBits
			memcpy(&this->gap_list.numBits, recv_data + data_index, sizeof(long));
			data_index	+= sizeof(long);

			//(bitmap) gapList.bitmap
			for(i=0; i<this->gap_list.numBits; i++)
			{
				tmp_char	= recv_data[data_index + i/8];

				if( (tmp_char & (0x80 >> i%8)) != 0 )
				{
					SequenceNumber_t*	recv_sn	= new SequenceNumber_t();
					SequenceNumber_t	cur_sn	= SequenceNumber_t(0, i);

					*recv_sn	= this->gap_list.base + cur_sn;

					this->gap_list.set.insertInRear(recv_sn);
				}
			}
			data_index	+= ((this->gap_list.numBits+31)/32) * 4/*the size of one bitmap block*/;
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

			//(SequenceNumber_t) gapStart
			memcpy(&this->gap_start.value, recv_data + data_index, sizeof(SequenceNumber_t));
			data_index	+= sizeof(SequenceNumber_t);
			ByteStream::ChangeByteOrdering(&this->gap_start.value.high, sizeof(long));
			ByteStream::ChangeByteOrdering(&this->gap_start.value.low, sizeof(long));

			//(SequenceNumber_t) gapList.base
			memcpy(&this->gap_list.base, recv_data + data_index, sizeof(SequenceNumber_t));
			data_index	+= sizeof(SequenceNumber_t);
			ByteStream::ChangeByteOrdering(&this->gap_list.base.high, sizeof(long));
			ByteStream::ChangeByteOrdering(&this->gap_list.base.low, sizeof(long));

			//(unsigned long) gapList.numBits
			memcpy(&this->gap_list.numBits, recv_data + data_index, sizeof(long));
			data_index	+= sizeof(long);
			ByteStream::ChangeByteOrdering(&this->gap_list.numBits, sizeof(long));

			//(bitmap) gapList.bitmap
			for(i=0; i<this->gap_list.numBits; i++)
			{
				tmp_char	= recv_data[data_index + i/8];

				if( (tmp_char & (0x80 >> i%8)) != 0 )
				{
					SequenceNumber_t*	recv_sn	= new SequenceNumber_t();
					SequenceNumber_t	cur_sn	= SequenceNumber_t(0, i);

					*recv_sn	= this->gap_list.base + cur_sn;

					this->gap_list.set.insertInRear(recv_sn);
				}
			}
			data_index	+= ((this->gap_list.numBits+31)/32) * 4/*the size of one bitmap block*/;
		}
	}
}
