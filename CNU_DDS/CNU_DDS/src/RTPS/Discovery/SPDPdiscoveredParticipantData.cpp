#include "../../../include/RTPS/Discovery/SPDPdiscoveredParticipantData.h"
#include "../../../include/RTPS/Messages/SubmessageElements/ParameterList.h"
#include "../../../include/Utility/SystemInformation.h"
#include "../../../include/RTPS/Structure/Participant.h"

namespace CNU_DDS
{
	ByteStream*	SPDPdiscoveredParticipantData::Serialize()
	{
		ByteStream*			stream								= new ByteStream();
		unsigned char		data[ByteStream::DATA_LENGTH_MAX]	= {0,};
		unsigned long		data_index							= 0;
		int					i									= 0;
		ByteOrderingType	ordering							= SystemInformation::getSystemInformationInstance()->getSystemByteOrderingType();

		*((short*)(data+data_index))	= PID_PARTICIPANT_GUID.value;
		data_index	+= 2;
		*((short*)(data+data_index))	= sizeof(GUID_t);
		data_index	+= 2;
		memcpy(data+data_index, &this->guid_prefix, sizeof(GUID_t));
		data_index	+= sizeof(GuidPrefix_t);
		memcpy(data+data_index, &ENTITYID_PARTICIPANT, sizeof(EntityId_t));
		data_index	+= sizeof(EntityId_t);

		*((short*)(data+data_index))	= PID_BUILTIN_ENDPOINT_SET.value;
		data_index	+= 2;
		*((short*)(data+data_index))	= 4;
		data_index	+= 2;
		memcpy(data+data_index, &available_builtin_endpoints, 4);
		data_index	+= 4;

		*((short*)(data+data_index))	= PID_PROTOCOL_VERSION.value;
		data_index	+= 2;
		*((short*)(data+data_index))	= 4;
		data_index	+= 2;
		memcpy(data+data_index, &protocol_version, sizeof(ProtocolVersion_t));
		data_index	+= 4;

		*((short*)(data+data_index))	= PID_VENDORID.value;
		data_index	+= 2;
		*((short*)(data+data_index))	= 4;
		data_index	+= 2;
		memcpy(data+data_index, &vendor_id, sizeof(VendorId_t));
		data_index	+= 4;

		for(i=1; i<=default_unicast_locator_list->getSize(); i++)
		{
			*((short*)(data+data_index))	= PID_DEFAULT_UNICAST_LOCATOR.value;
			data_index	+= 2;
			*((short*)(data+data_index))	= sizeof(Locator_t);
			data_index	+= 2;
			memcpy(data+data_index, default_unicast_locator_list->getNodeByIndex(i)->value, sizeof(Locator_t));
			data_index	+= sizeof(Locator_t);
		}

		for(i=1; i<=default_multicast_locator_list->getSize(); i++)
		{
			*((short*)(data+data_index))	= PID_DEFAULT_MULTICAST_LOCATOR.value;
			data_index	+= 2;
			*((short*)(data+data_index))	= sizeof(Locator_t);
			data_index	+= 2;
			memcpy(data+data_index, default_multicast_locator_list->getNodeByIndex(i)->value, sizeof(Locator_t));
			data_index	+= sizeof(Locator_t);
		}

		for(i=1; i<=metatraffic_unicast_locator_list->getSize(); i++)
		{
			*((short*)(data+data_index))	= PID_METATRAFFIC_UNICAST_LOCATOR.value;
			data_index	+= 2;
			*((short*)(data+data_index))	= sizeof(Locator_t);
			data_index	+= 2;
			memcpy(data+data_index, metatraffic_unicast_locator_list->getNodeByIndex(i)->value, sizeof(Locator_t));
			data_index	+= sizeof(Locator_t);
		}

		for(i=1; i<=metatraffic_multicast_locator_list->getSize(); i++)
		{
			*((short*)(data+data_index))	= PID_METATRAFFIC_MULTICAST_LOCATOR.value;
			data_index	+= 2;
			*((short*)(data+data_index))	= sizeof(Locator_t);
			data_index	+= 2;
			memcpy(data+data_index, metatraffic_multicast_locator_list->getNodeByIndex(i)->value, sizeof(Locator_t));
			data_index	+= sizeof(Locator_t);
		}

		*((short*)(data+data_index))	= PID_PARTICIPANT_LEASE_DURATION.value;
		data_index	+= 2;
		*((short*)(data+data_index))	= sizeof(Duration_t);
		data_index	+= 2;
		memcpy(data+data_index, &lease_duration, sizeof(Duration_t));
		data_index	+= sizeof(Duration_t);

		*((short*)(data+data_index))	= PID_SENTINEL.value;
		data_index	+= 2;
		*((short*)(data+data_index))	= 0;
		data_index	+= 2;

		if(ordering == BYTE_ORDERING_TYPE_BIG_ENDIAN)
		{
			stream->setData(BYTE_ORDERING_TYPE_BIG_ENDIAN_LIST, data, data_index);
		}
		else
		{
			stream->setData(BYTE_ORDERING_TYPE_LITTLE_ENDIAN_LIST, data, data_index);
		}

		return stream;
	}

	void		SPDPdiscoveredParticipantData::Deserialize(ByteStream* stream)
	{
		unsigned char*		data		= stream->getData();
		unsigned long		data_index	= 0;
		ByteOrderingType	ordering	= SystemInformation::getSystemInformationInstance()->getSystemByteOrderingType();
		ByteOrderingType	recv_ordering;
		short*				parameter_id;
		short*				parameter_length;
		Locator_t*			locator;

		default_unicast_locator_list		= new LocatorSeq();
		default_multicast_locator_list		= new LocatorSeq();
		metatraffic_unicast_locator_list	= new LocatorSeq();
		metatraffic_multicast_locator_list	= new LocatorSeq();

		if((stream->getByteOrderingType() == BYTE_ORDERING_TYPE_BIG_ENDIAN) || (stream->getByteOrderingType() == BYTE_ORDERING_TYPE_BIG_ENDIAN_LIST))
		{
			recv_ordering	= BYTE_ORDERING_TYPE_BIG_ENDIAN;
		}
		else
		{
			recv_ordering	= BYTE_ORDERING_TYPE_LITTLE_ENDIAN;
		}

		if(ordering == recv_ordering)
		{
			do
			{
				parameter_id		= (short*)(data + data_index);
				parameter_length	= (short*)(data + data_index + 2);
				data_index	+= 4;

				if(*parameter_id == PID_PARTICIPANT_GUID.value)
				{
					memcpy(&guid_prefix, data+data_index, sizeof(GuidPrefix_t));
				}
				else if(*parameter_id == PID_BUILTIN_ENDPOINT_SET.value)
				{
					memcpy(&available_builtin_endpoints, data+data_index, *parameter_length);
				}
				else if(*parameter_id == PID_PROTOCOL_VERSION.value)
				{
					memcpy(&protocol_version, data+data_index, sizeof(ProtocolVersion_t));
				}
				else if(*parameter_id == PID_VENDORID.value)
				{
					memcpy(&vendor_id, data+data_index, sizeof(vendor_id));
				}
				else if(*parameter_id == PID_DEFAULT_UNICAST_LOCATOR.value)
				{
					locator	= new Locator_t();
					memcpy(locator, data+data_index, *parameter_length);
					if(locator->kind	== LOCATOR_KIND_UDPv4)
					{
						default_unicast_locator_list->insertInRear(locator);
					}
					else
					{
						delete(locator);
					}
				}
				else if(*parameter_id == PID_DEFAULT_MULTICAST_LOCATOR.value)
				{
					locator	= new Locator_t();
					memcpy(locator, data+data_index, *parameter_length);
					
					if(locator->kind	== LOCATOR_KIND_UDPv4)
					{
						default_multicast_locator_list->insertInRear(locator);
					}
					else
					{
						delete(locator);
					}
				}
				else if(*parameter_id == PID_METATRAFFIC_UNICAST_LOCATOR.value)
				{
					locator	= new Locator_t();
					memcpy(locator, data+data_index, *parameter_length);
					
					if(locator->kind	== LOCATOR_KIND_UDPv4)
					{
						metatraffic_unicast_locator_list->insertInRear(locator);	
					}
					else
					{
						delete(locator);
					}
				}
				else if(*parameter_id == PID_METATRAFFIC_MULTICAST_LOCATOR.value)
				{
					locator	= new Locator_t();
					memcpy(locator, data+data_index, *parameter_length);
					
					if(locator->kind	== LOCATOR_KIND_UDPv4)
					{
						metatraffic_multicast_locator_list->insertInRear(locator);	
					}
					else
					{
						delete(locator);
					}
				}
				else if(*parameter_id == PID_PARTICIPANT_LEASE_DURATION.value)
				{
					memcpy(&lease_duration, data+data_index, *parameter_length);
				}

				data_index	+= ((short)(*parameter_length));

			}while((data_index < stream->getLength()) && (*parameter_id != PID_SENTINEL.value));
		}
		else
		{
			do
			{
				parameter_id		= (short*)(data + data_index);
				parameter_length	= (short*)(data + data_index + 2);
				data_index	+= 4;

				ByteStream::ChangeByteOrdering(parameter_id, sizeof(short));
				ByteStream::ChangeByteOrdering(parameter_length, sizeof(short));

				if(*parameter_id == PID_PARTICIPANT_GUID.value)
				{
					memcpy(&guid_prefix, data+data_index, sizeof(GuidPrefix_t));
				}
				else if(*parameter_id == PID_BUILTIN_ENDPOINT_SET.value)
				{
					memcpy(&available_builtin_endpoints, data+data_index, *parameter_length);
				}
				else if(*parameter_id == PID_PROTOCOL_VERSION.value)
				{
					memcpy(&protocol_version, data+data_index, sizeof(ProtocolVersion_t));
				}
				else if(*parameter_id == PID_VENDORID.value)
				{
					memcpy(&vendor_id, data+data_index, sizeof(VendorId_t));
				}
				else if(*parameter_id == PID_DEFAULT_UNICAST_LOCATOR.value)
				{
					locator	= new Locator_t();
					memcpy(locator, data+data_index, *parameter_length);
					ByteStream::ChangeByteOrdering(&locator->kind, sizeof(long));
					ByteStream::ChangeByteOrdering(&locator->port, sizeof(unsigned long));
					
					if(locator->kind	== LOCATOR_KIND_UDPv4)
					{
						default_unicast_locator_list->insertInRear(locator);	
					}
					else
					{
						delete(locator);
					}
				}
				else if(*parameter_id == PID_DEFAULT_MULTICAST_LOCATOR.value)
				{
					locator	= new Locator_t();
					memcpy(locator, data+data_index, *parameter_length);
					ByteStream::ChangeByteOrdering(&locator->kind, sizeof(long));
					ByteStream::ChangeByteOrdering(&locator->port, sizeof(unsigned long));
					
					if(locator->kind	== LOCATOR_KIND_UDPv4)
					{
						default_multicast_locator_list->insertInRear(locator);	
					}
					else
					{
						delete(locator);
					}
				}
				else if(*parameter_id == PID_METATRAFFIC_UNICAST_LOCATOR.value)
				{
					locator	= new Locator_t();
					memcpy(locator, data+data_index, *parameter_length);
					ByteStream::ChangeByteOrdering(&locator->kind, sizeof(long));
					ByteStream::ChangeByteOrdering(&locator->port, sizeof(unsigned long));
					
					if(locator->kind	== LOCATOR_KIND_UDPv4)
					{
						metatraffic_unicast_locator_list->insertInRear(locator);	
					}
					else
					{
						delete(locator);
					}
				}
				else if(*parameter_id == PID_METATRAFFIC_MULTICAST_LOCATOR.value)
				{
					locator	= new Locator_t();
					memcpy(locator, data+data_index, *parameter_length);
					ByteStream::ChangeByteOrdering(&locator->kind, sizeof(long));
					ByteStream::ChangeByteOrdering(&locator->port, sizeof(unsigned long));
					
					if(locator->kind	== LOCATOR_KIND_UDPv4)
					{
						metatraffic_multicast_locator_list->insertInRear(locator);	
					}
					else
					{
						delete(locator);
					}
				}
				else if(*parameter_id == PID_PARTICIPANT_LEASE_DURATION.value)
				{
					memcpy(&lease_duration, data+data_index, *parameter_length);
					ByteStream::ChangeByteOrdering(&lease_duration.sec, sizeof(long));
					ByteStream::ChangeByteOrdering(&lease_duration.nanosec, sizeof(unsigned long));
				}

				data_index	+= (short)(*parameter_length);

			}while((data_index < stream->getLength()) && (*parameter_id != PID_SENTINEL.value));
		}
	}

	ParticipantProxy*	SPDPdiscoveredParticipantData ::getParticipantProxy()
	{
		unsigned long	result	= (unsigned long)this;
		result	+= sizeof(ParticipantBuiltinTopicData);
		return (ParticipantProxy*)result;
	}
}
