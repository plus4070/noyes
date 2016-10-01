#include "../../../../include/RTPS/Messages/Submessages/AckNack.h"
#include "../../../../include/Utility/SystemInformation.h"

namespace CNU_DDS
{
	AckNack::AckNack()
	{

	}

	AckNack::~AckNack()
	{

	}

	ByteStream* AckNack::Serialize()
	{
		ByteStream* 		new_stream 								= new ByteStream();
		unsigned char 		tmp_data[ByteStream::DATA_LENGTH_MAX]	= { 0, };
		SystemInformation*	si 										= SystemInformation::getSystemInformationInstance();
		unsigned long		data_index 								= 0;

		/*
		 * Submessage part
		 */

		//submessage header
		data_index	= sizeof(SubmessageHeader);

		//readerId
		memcpy(tmp_data + data_index, &this->reader_id.value, sizeof(EntityId_t));
		data_index	+= sizeof(EntityId_t);

		//writerId
		memcpy(tmp_data + data_index, &this->writer_id.value, sizeof(EntityId_t));
		data_index	+= sizeof(EntityId_t);

		//SequenceNumberSet.bitmapBase
		memcpy(tmp_data + data_index, &this->reader_sn_state.base, sizeof(SequenceNumber_t));
		data_index	+= sizeof(SequenceNumber_t);

		//SequenceNumberSet.numBits
		unsigned long	tmp_long	= this->reader_sn_state.numBits;
		//unsigned long	tmp_count	= 0;
		unsigned long	tmp_addr	= data_index;
		//memcpy(tmp_data + data_index, &tmp_long, sizeof(long));
		data_index	+= sizeof(long);

		unsigned char*	tmp_bitmap	= tmp_data + data_index;
		int		i, j;

		if(tmp_long	> 256)
		{
			tmp_long = 255;
			memset(tmp_bitmap, 0x00, 32);
		}
		else if((0 < tmp_long) && (tmp_long < 256))
		{
			// Set the bitmap to 1
			memset(tmp_bitmap, 0xFF, 32);

			// Set the bitmap to 0
			for(i=0; i<32; i++)
			{
				if(tmp_long/(8*(i+1)) > 0)
				{
					tmp_bitmap[i]	= 0;
				}
				else if(tmp_long%8 > 0)
				{
					for(j=0; j<tmp_long%8; j++)
					{
						tmp_bitmap[i] ^= (0x80 >> (j));
					}
					break;
				}
				else
				{
					break;
				}
			}
		}

		memcpy(tmp_data + tmp_addr, &tmp_long, sizeof(long));

		//SequenceNumberSet.bitmap (setup)
		while(this->reader_sn_state.set.getSize() > 0)
		{
			SequenceNumber_t*	cur_seq		= this->reader_sn_state.set.popFirst();
			SequenceNumber_t	calc_seq;
			calc_seq.high	= cur_seq->high - this->reader_sn_state.base.high;
			calc_seq.low	= cur_seq->low - this->reader_sn_state.base.low;

			if(calc_seq.high == 0 && calc_seq.low < 256)
			{
				tmp_bitmap[calc_seq.low/8] |= (0x80 >> (calc_seq.low%8));
			}
			else
			{
				// do nothing
			}
		}

		if(this->reader_sn_state.numBits < 256)
		{
			data_index	+= ((tmp_long+31)/32) * 4;	//the size of one bitmap block
		}
		else
		{
			data_index	+= 32;	//the size of one bitmap block
		}

		//SequenceNumberSet.bitmap
		/*
		while(this->reader_sn_state.set.getSize() > 0)
		{
			SequenceNumber_t*	cur_seq		= this->reader_sn_state.set.popFirst();
			SequenceNumber_t	calc_seq;
			calc_seq.high	= cur_seq->high - this->reader_sn_state.base.high;
			calc_seq.low	= cur_seq->low - this->reader_sn_state.base.low;

			if(calc_seq.high == 0 && calc_seq.low < 256)
			{
				tmp_data[data_index + calc_seq.low/8] |= (0x80 >> (calc_seq.low%8));
				tmp_count++;
			}
			else
			{
				// do nothing
			}

			//Heap Error!!!!!!!!!!
			//delete(cur_seq);
		}
		if(this->reader_sn_state.numBits < 256)
		{
			data_index	+= ((this->reader_sn_state.numBits+31)/32) * 4 //the size of one bitmap block;
		}
		else
		{
			data_index	+= 32//the size of one bitmap block;
		}
		memcpy(tmp_data + tmp_addr, &tmp_count, sizeof(long));
		*/
		//Count
		memcpy(tmp_data + data_index, &this->count.value, sizeof(Count_t));
		data_index	+= sizeof(Count_t);

		/*
		 * SubmessageHeader part
		 */
		this->submessage_header.submessage_id		= SUBMESSAGE_KIND_ACKNACK;
		this->submessage_header.submessage_length	= data_index - sizeof(SubmessageHeader);

		if (si->getSystemByteOrderingType() == BYTE_ORDERING_TYPE_LITTLE_ENDIAN)
		{
			this->submessage_header.flags |= 0x01;
		}

		memcpy(tmp_data, &this->submessage_header, sizeof(SubmessageHeader));
		new_stream->setData(si->getSystemByteOrderingType(), tmp_data, data_index);
		return new_stream;
	}

	void AckNack::Deserialize(unsigned char* data)
	{
		SystemInformation*	si 				= SystemInformation::getSystemInformationInstance();
		unsigned char*		recv_data		= data;
		unsigned long		data_index		= 0;
		unsigned long		i				= 0;
		unsigned char		tmp_char		= 0;
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

			//(SequenceNumber_t) readerSNState.base
			memcpy(&this->reader_sn_state.base, recv_data + data_index, sizeof(SequenceNumber_t));
			data_index	+= sizeof(SequenceNumber_t);

			//(unsigned long) readerSNState.numBits
			memcpy(&this->reader_sn_state.numBits, recv_data + data_index, sizeof(long));
			data_index	+= sizeof(long);

			//(bit) readerSNState.bitmap
			for(i=0; i<this->reader_sn_state.numBits; i++)
			{
				tmp_char	= recv_data[data_index + i/8];

				if( (tmp_char & (0x80 >> i%8)) != 0 )
				{
					SequenceNumber_t*	recv_sn	= new SequenceNumber_t();
					SequenceNumber_t	cur_sn	= SequenceNumber_t(0, i);

					*recv_sn	= this->reader_sn_state.base + cur_sn;

					this->reader_sn_state.set.insertInRear(recv_sn);
				}
			}
			data_index	+= ((this->reader_sn_state.numBits+31)/32) * 4/*the size of one bitmap block*/;

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

			//(SequenceNumber_t) readerSNState.base
			memcpy(&this->reader_sn_state.base, recv_data + data_index, sizeof(SequenceNumber_t));
			data_index	+= sizeof(SequenceNumber_t);
			ByteStream::ChangeByteOrdering(&this->reader_sn_state.base.high, sizeof(long));
			ByteStream::ChangeByteOrdering(&this->reader_sn_state.base.low, sizeof(long));

			//(unsigned long) readerSNState.numBits
			memcpy(&this->reader_sn_state.numBits, recv_data + data_index, sizeof(long));
			data_index	+= sizeof(long);
			ByteStream::ChangeByteOrdering(&this->reader_sn_state.numBits, sizeof(long));

			//(bit) readerSNState.bitmap
			for(i=0; i<this->reader_sn_state.numBits; i++)
			{
				tmp_char	= recv_data[data_index + i/8];

				if( (tmp_char & (0x80 >> i%8)) != 0 )
				{
					SequenceNumber_t*	recv_sn	= new SequenceNumber_t();
					SequenceNumber_t	cur_sn	= SequenceNumber_t(0, i);

					*recv_sn	= this->reader_sn_state.base + cur_sn;

					this->reader_sn_state.set.insertInRear(recv_sn);
				}
			}
			data_index	+= ((this->reader_sn_state.numBits+31)/32) * 4/*the size of one bitmap block*/;

			//(Count_t) count
			memcpy(&this->count.value, recv_data + data_index, sizeof(Count_t));
			data_index	+= sizeof(Count_t);
			ByteStream::ChangeByteOrdering(&this->count.value.value, sizeof(long));
		}
	}
}
