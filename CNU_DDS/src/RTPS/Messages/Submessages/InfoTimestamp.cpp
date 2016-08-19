#include "../../../../include/RTPS/Messages/Submessages/InfoTimestamp.h"
#include "../../../../include/Utility/SystemInformation.h"

namespace CNU_DDS
{
	InfoTimestamp::InfoTimestamp()
	{

	}

	InfoTimestamp::~InfoTimestamp()
	{

	}

	ByteStream*	InfoTimestamp::Serialize()
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

		//timestamp
		memcpy(tmp_data + data_index, &this->timestamp.value, sizeof(Time_t));
		data_index	+= sizeof(Time_t);

		/*
		 * SubmessageHeader part
		 */
		this->submessage_header.submessage_id		= SUBMESSAGE_KIND_INFO_TIMESTAMP;
		this->submessage_header.submessage_length	= data_index - sizeof(SubmessageHeader);

		if (si->getSystemByteOrderingType() == BYTE_ORDERING_TYPE_LITTLE_ENDIAN)
		{
			this->submessage_header.flags |= 0x01;
		}

		memcpy(tmp_data, &this->submessage_header, sizeof(SubmessageHeader));
		new_stream->setData(si->getSystemByteOrderingType(), tmp_data, data_index);
		return new_stream;
	}

	void		InfoTimestamp::Deserialize(unsigned char*	data)
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

			//(Time_t) timestamp ( only if I-bit(0x02) )
			memcpy(&this->timestamp.value, recv_data + data_index, sizeof(Time_t));
			data_index	+= sizeof(Time_t);
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

			//(Time_t) timestamp ( only if I-bit(0x02) )
			memcpy(&this->timestamp.value, recv_data + data_index, sizeof(Time_t));
			data_index	+= sizeof(Time_t);
			ByteStream::ChangeByteOrdering(&this->timestamp.value.sec, sizeof(long));
			ByteStream::ChangeByteOrdering(&this->timestamp.value.nanosec, sizeof(long));
		}
	}
}
