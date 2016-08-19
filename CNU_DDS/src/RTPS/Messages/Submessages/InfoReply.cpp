#include "../../../../include/RTPS/Messages/Submessages/InfoReply.h"
#include "../../../../include/Utility/SystemInformation.h"

namespace CNU_DDS
{
	InfoReply::InfoReply()
	{

	}

	InfoReply::~InfoReply()
	{

	}

	ByteStream*	InfoReply::Serialize()
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

		//unicastLocatorList
		//numLocaotrs
		unsigned long	tmp_long	= 0;
		unsigned long	i			= 0;
		tmp_long	= this->unicast_locator_list.value.getSize();
		memcpy(tmp_data + data_index, &tmp_long, sizeof(long));
		data_index	+= sizeof(long);

		//locator
		for(i=0; i<tmp_long; i++)
		{
			memcpy(tmp_data + data_index, this->unicast_locator_list.value.getNodeByIndex(i+1)->value, sizeof(Locator_t));
			data_index	+= sizeof(Locator_t);
		}

		//multicastLocaotrList ( only if M-bit(0x02) )
		if(this->submessage_header.flags | 0x02 == 0x02)
		{
			//numLocaotrs
			tmp_long	= this->multicast_locator_list.value.getSize();
			memcpy(tmp_data + data_index, &tmp_long, sizeof(long));
			data_index	+= sizeof(long);

			//locator
			for(i=0; i<tmp_long; i++)
			{
				memcpy(tmp_data + data_index, this->multicast_locator_list.value.getNodeByIndex(i+1)->value, sizeof(Locator_t));
				data_index	+= sizeof(Locator_t);
			}
		}

		/*
		 * SubmessageHeader part
		 */
		this->submessage_header.submessage_id		= SUBMESSAGE_KIND_INFO_REPLY;
		this->submessage_header.submessage_length	= data_index - sizeof(SubmessageHeader);

		if (si->getSystemByteOrderingType() == BYTE_ORDERING_TYPE_LITTLE_ENDIAN)
		{
			this->submessage_header.flags |= 0x01;
		}

		memcpy(tmp_data, &this->submessage_header, sizeof(SubmessageHeader));
		new_stream->setData(si->getSystemByteOrderingType(), tmp_data, data_index);
		return new_stream;
	}

	void		InfoReply::Deserialize(unsigned char*	data)
	{
		SystemInformation*	si 				= SystemInformation::getSystemInformationInstance();
		unsigned long		tmp_long		= 0;
		unsigned long		i				= 0;
		Locator_t*			recv_locator	= NULL;
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

			//(unsigned long) unicastLocatorList.numLocators
			memcpy(&tmp_long, recv_data + data_index, sizeof(long));
			data_index	+= sizeof(long);

			//(Locator_t) unicastLocatorList.locator
			for(i=0; i<tmp_long; i++)
			{
				recv_locator	= new Locator_t();

				memcpy(recv_locator, recv_data + data_index, sizeof(Locator_t));
				data_index	+= sizeof(Locator_t);

				this->unicast_locator_list.value.insertInRear(recv_locator);
			}

			//multicastLocaotrList ( only if M-bit(0x02) )
			if(this->submessage_header.flags | 0x02 == 0x02)
			{
				//(unsigned long) multicastLocatorList.numLocators
				memcpy(&tmp_long, recv_data + data_index, sizeof(long));
				data_index	+= sizeof(long);

				//(Locator_t) multicastLocatorList.locator
				for(i=0; i<tmp_long; i++)
				{
					recv_locator	= new Locator_t();

					memcpy(recv_locator, recv_data + data_index, sizeof(Locator_t));
					data_index	+= sizeof(Locator_t);

					this->multicast_locator_list.value.insertInRear(recv_locator);
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

			//(unsigned long) unicastLocatorList.numLocators
			memcpy(&tmp_long, recv_data + data_index, sizeof(long));
			data_index	+= sizeof(long);
			ByteStream::ChangeByteOrdering(&tmp_long, sizeof(long));

			//(Locator_t) unicastLocatorList.locator
			for(i=0; i<tmp_long; i++)
			{
				recv_locator	= new Locator_t();

				memcpy(recv_locator, recv_data + data_index, sizeof(Locator_t));
				data_index	+= sizeof(Locator_t);
				ByteStream::ChangeByteOrdering(&recv_locator->kind, sizeof(long));
				ByteStream::ChangeByteOrdering(&recv_locator->port, sizeof(long));

				this->unicast_locator_list.value.insertInRear(recv_locator);
			}

			//multicastLocaotrList ( only if M-bit(0x02) )
			if(this->submessage_header.flags | 0x02 == 0x02)
			{
				//(unsigned long) multicastLocatorList.numLocators
				memcpy(&tmp_long, recv_data + data_index, sizeof(long));
				data_index	+= sizeof(long);
				ByteStream::ChangeByteOrdering(&tmp_long, sizeof(long));

				//(Locator_t) multicastLocatorList.locator
				for(i=0; i<tmp_long; i++)
				{
					recv_locator	= new Locator_t();

					memcpy(recv_locator, recv_data + data_index, sizeof(Locator_t));
					data_index	+= sizeof(Locator_t);
					ByteStream::ChangeByteOrdering(&recv_locator->kind, sizeof(long));
					ByteStream::ChangeByteOrdering(&recv_locator->port, sizeof(long));

					this->multicast_locator_list.value.insertInRear(recv_locator);
				}
			}
		}
	}
}
