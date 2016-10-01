#include "../../../../include/RTPS/Messages/Submessages/InfoSource.h"
#include "../../../../include/Utility/SystemInformation.h"

namespace CNU_DDS
{
	InfoSource::InfoSource()
	{

	}

	InfoSource::~InfoSource()
	{

	}

	ByteStream*	InfoSource::Serialize()
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

		//unused
		data_index	+= 4;

		//version
		memcpy(tmp_data + data_index, &this->protocol_version.value, sizeof(ProtocolVersion_t));
		data_index	+= sizeof(ProtocolVersion_t);

		//vendorId
		memcpy(tmp_data + data_index, &this->vendor_id.value, sizeof(VendorId_t));
		data_index	+= sizeof(VendorId_t);

		//guidprefix
		memcpy(tmp_data + data_index, &this->guid_prefix.value, sizeof(GuidPrefix_t));
		data_index	+= sizeof(GuidPrefix_t);

		/*
		 * SubmessageHeader part
		 */
		this->submessage_header.submessage_id		= SUBMESSAGE_KIND_INFO_SOURRCE;
		this->submessage_header.submessage_length	= data_index - sizeof(SubmessageHeader);

		if (si->getSystemByteOrderingType() == BYTE_ORDERING_TYPE_LITTLE_ENDIAN)
		{
			this->submessage_header.flags |= 0x01;
		}

		memcpy(tmp_data, &this->submessage_header, sizeof(SubmessageHeader));
		new_stream->setData(si->getSystemByteOrderingType(), tmp_data, data_index);
		return new_stream;
	}

	void		InfoSource::Deserialize(unsigned char*	data)
	{
		SystemInformation*	si 				= SystemInformation::getSystemInformationInstance();
		unsigned char*		recv_data	= data;
		unsigned long		data_index	= 0;
		ByteOrderingType	recv_ordering	= (recv_data[1] & 0x01)?BYTE_ORDERING_TYPE_LITTLE_ENDIAN:BYTE_ORDERING_TYPE_BIG_ENDIAN;

		//Header
		//SubmessageKind
		this->submessage_header.submessage_id	= recv_data[data_index];
		data_index	+= 1;

		this->submessage_header.flags			= recv_data[data_index];
		data_index	+= 1;

		memcpy(&this->submessage_header.submessage_length, recv_data + data_index, sizeof(short));
		data_index	+= sizeof(short);

		if(si->getSystemByteOrderingType() == recv_ordering)
		{
			ByteStream::ChangeByteOrdering(&this->submessage_header.submessage_length, sizeof(short));
		}

		//(4 bytes) unused
		data_index	+= 4;

		//(ProtocolVersion_t) version
		memcpy(&this->protocol_version.value, recv_data + data_index, sizeof(ProtocolVersion_t));
		data_index	+= sizeof(ProtocolVersion_t);

		//(VendorId_t) vendorId
		memcpy(&this->vendor_id.value, recv_data + data_index, sizeof(VendorId_t));
		data_index	+= sizeof(VendorId_t);

		//(GuidPrefix_t) guidprefix
		memcpy(&this->guid_prefix.value, recv_data + data_index, sizeof(GuidPrefix_t));
		data_index	+= sizeof(GuidPrefix_t);
	}
}
