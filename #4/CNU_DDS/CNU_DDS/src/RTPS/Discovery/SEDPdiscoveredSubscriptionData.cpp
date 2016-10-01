#include "../../../include/RTPS/Discovery/SEDPdiscoveredSubscriptionData.h"
#include "../../../include/RTPS/Messages/SubmessageElements/ParameterList.h"
#include "../../../include/Utility/SystemInformation.h"
#include "../../../include/RTPS/Structure/Participant.h"
#include <string.h>
#include <stdio.h>

namespace CNU_DDS
{
	SEDPdiscoveredSubscriptionData::SEDPdiscoveredSubscriptionData()
	{
		//memset(this, 0, sizeof(SubscriptionBuiltinTopicData));
	}

	SEDPdiscoveredSubscriptionData::~SEDPdiscoveredSubscriptionData()
	{

	}

	ReaderProxy*	SEDPdiscoveredSubscriptionData::getReaderProxyInstance()
	{
		unsigned long result	= (unsigned long)this;
		result	+= sizeof(SubscriptionBuiltinTopicData);

		return (ReaderProxy*)result;
	}

	ByteStream*	SEDPdiscoveredSubscriptionData::Serialize()
	{
		ByteStream*			stream								= new ByteStream();
		unsigned char		data[ByteStream::DATA_LENGTH_MAX]	= {0,};
		unsigned long		data_index							= 0;
		int					i									= 0;
		short				parameter_length					= 0;
		unsigned long		str_len								= 0;
		ByteOrderingType	ordering							= SystemInformation::getSystemInformationInstance()->getSystemByteOrderingType();

		*((short*)(data+data_index))	= PID_ENDPOINT_GUID.value;
		data_index	+= 2;
		*((short*)(data+data_index))	= sizeof(GUID_t);
		data_index	+= 2;
		memcpy(data+data_index, &remote_reader_guid, sizeof(GUID_t));
		data_index	+= sizeof(GUID_t);

		*((short*)(data+data_index))	= PID_TOPIC_NAME.value;
		data_index	+= 2;
		str_len							= strlen(topic_name.c_str()) + 1/*NULL*/;
		parameter_length				= str_len + (str_len%4) + 4/*string_length*/;
		*((short*)(data+data_index))	= parameter_length;
		data_index	+= 2;
		memcpy(data+data_index, &str_len, 4);
		data_index	+= 4;
		memcpy(data+data_index, topic_name.c_str(), str_len-1);
		data_index	+= parameter_length - 4;

		*((short*)(data+data_index))	= PID_TYPE_NAME.value;
		data_index	+= 2;
		str_len							= strlen(type_name.c_str()) + 1/*NULL*/;
		parameter_length				= str_len + (str_len%4) + 4/*string_length*/;
		*((short*)(data+data_index))	= parameter_length;
		data_index	+= 2;
		memcpy(data+data_index, &str_len, 4);
		data_index	+= 4;
		memcpy(data+data_index, type_name.c_str(), str_len-1);
		data_index	+= parameter_length - 4;

		*((short*)(data+data_index))	= PID_RELIABILITY.value;
		data_index	+= 2;
		*((short*)(data+data_index))	= sizeof(ReliabilityQosPolicy);
		data_index	+= 2;
		memcpy(data+data_index, &reliability, sizeof(ReliabilityQosPolicy));
		data_index	+= sizeof(ReliabilityQosPolicy);

		*((short*)(data+data_index))	= PID_OWNERSHIP.value;
		data_index	+= 2;
		*((short*)(data+data_index))	= sizeof(OwnershipQosPolicy);
		data_index	+= 2;
		memcpy(data+data_index, &ownership, sizeof(OwnershipQosPolicy));
		data_index	+= sizeof(OwnershipQosPolicy);

		*((short*)(data+data_index))	= PID_LIVELINESS.value;
		data_index	+= 2;
		*((short*)(data+data_index))	= sizeof(LivelinessQosPolicy);
		data_index	+= 2;
		memcpy(data+data_index, &liveliness, sizeof(LivelinessQosPolicy));
		data_index	+= sizeof(LivelinessQosPolicy);

		*((short*)(data+data_index))	= PID_DURABILITY.value;
		data_index	+= 2;
		*((short*)(data+data_index))	= sizeof(DurabilityQosPolicy);
		data_index	+= 2;
		memcpy(data+data_index, &durability, sizeof(DurabilityQosPolicy));
		data_index	+= sizeof(DurabilityQosPolicy);
		/*
		*((short*)(data+data_index))	= PID_DURABILITY_SERVICE.value;
		data_index	+= 2;
		*((short*)(data+data_index))	= sizeof(DurabilityServiceQosPolicy);
		data_index	+= 2;
		memcpy(data+data_index, &durability_service, sizeof(DurabilityServiceQosPolicy));
		data_index	+= sizeof(DurabilityServiceQosPolicy);
		*/
		/*
		*((short*)(data+data_index))	= PID_PRESENTATION.value;
		data_index	+= 2;
		*((short*)(data+data_index))	= sizeof(PresentationQosPolicy) + 2 //dummy;
		data_index	+= 2;
		memcpy(data+data_index, &presentation, sizeof(PresentationQosPolicy));
		data_index	+= (sizeof(PresentationQosPolicy) + 2);
		*/
		*((short*)(data+data_index))	= PID_DESTINATION_ORDER.value;
		data_index	+= 2;
		*((short*)(data+data_index))	= sizeof(DestinationOrderQosPolicy);
		data_index	+= 2;
		memcpy(data+data_index, &destination_order, sizeof(DestinationOrderQosPolicy));
		data_index	+= sizeof(DestinationOrderQosPolicy);

		*((short*)(data+data_index))	= PID_DEADLINE.value;
		data_index	+= 2;
		*((short*)(data+data_index))	= sizeof(DeadlineQosPolicy);
		data_index	+= 2;
		memcpy(data+data_index, &deadline, sizeof(DeadlineQosPolicy));
		data_index	+= sizeof(DeadlineQosPolicy);

		*((short*)(data+data_index))	= PID_LATENCY_BUDGET.value;
		data_index	+= 2;
		*((short*)(data+data_index))	= sizeof(LatencyBudgetQosPolicy);
		data_index	+= 2;
		memcpy(data+data_index, &latency_budget, sizeof(LatencyBudgetQosPolicy));
		data_index	+= sizeof(LatencyBudgetQosPolicy);
		/*
		*((short*)(data+data_index))	= PID_LIFESPAN.value;
		data_index	+= 2;
		*((short*)(data+data_index))	= sizeof(LifespanQosPolicy);
		data_index	+= 2;
		memcpy(data+data_index, &lifespan, sizeof(LifespanQosPolicy));
		data_index	+= sizeof(LifespanQosPolicy);
		*/
		*((short*)(data+data_index))	= PID_PROTOCOL_VERSION.value;
		data_index	+= 2;
		*((short*)(data+data_index))	= 4;
		data_index	+= 2;
		memcpy(data+data_index, &PROTOCOL_VERSION, sizeof(ProtocolVersion_t));
		data_index	+= 4;

		*((short*)(data+data_index))	= PID_VENDORID.value;
		data_index	+= 2;
		*((short*)(data+data_index))	= 4;
		data_index	+= 2;
		data_index	+= 4;

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

	void		SEDPdiscoveredSubscriptionData::Deserialize(ByteStream* stream)
	{
		unsigned char*		data		= stream->getData();
		unsigned long		data_index	= 0;
		ByteOrderingType	ordering	= SystemInformation::getSystemInformationInstance()->getSystemByteOrderingType();
		ByteOrderingType	recv_ordering;
		short*				parameter_id;
		short*				parameter_length;
		Locator_t*			locator;

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

				if(*parameter_id == PID_ENDPOINT_GUID.value)
				{
					memcpy(&remote_reader_guid, data+data_index, sizeof(GUID_t));
				}
				else if(*parameter_id == PID_TOPIC_NAME.value)
				{
					topic_name	=  (const char*)(data + data_index + 4);
				}
				else if(*parameter_id == PID_TYPE_NAME.value)
				{
					type_name	=  (const char*)(data + data_index + 4);
				}
				else if(*parameter_id == PID_RELIABILITY.value)
				{
					memcpy(&reliability, data+data_index, sizeof(ReliabilityQosPolicy));
				}
				else if(*parameter_id == PID_OWNERSHIP.value)
				{
					memcpy(&ownership, data+data_index, sizeof(OwnershipQosPolicy));
				}
				else if(*parameter_id == PID_LIVELINESS.value)
				{
					memcpy(&liveliness, data+data_index, sizeof(LivelinessQosPolicy));
				}
				else if(*parameter_id == PID_DURABILITY.value)
				{
					memcpy(&durability, data+data_index, sizeof(DurabilityQosPolicy));
				}
				else if(*parameter_id == PID_DURABILITY_SERVICE.value)
				{
					memcpy(&durability_service, data+data_index, sizeof(DurabilityServiceQosPolicy));
				}
				else if(*parameter_id == PID_PRESENTATION.value)
				{
					memcpy(&presentation, data+data_index, sizeof(PresentationQosPolicy));
				}
				else if(*parameter_id == PID_DESTINATION_ORDER.value)
				{
					memcpy(&destination_order, data+data_index, sizeof(DestinationOrderQosPolicy));
				}
				else if(*parameter_id == PID_DEADLINE.value)
				{
					memcpy(&deadline, data+data_index, sizeof(DeadlineQosPolicy));
				}
				else if(*parameter_id == PID_LATENCY_BUDGET.value)
				{
					memcpy(&latency_budget, data+data_index, sizeof(LatencyBudgetQosPolicy));
				}
				else if(*parameter_id == PID_LIFESPAN.value)
				{
					memcpy(&lifespan, data+data_index, sizeof(LifespanQosPolicy));
				}

				data_index	+= ((short)(*parameter_length));

			}while(*parameter_id != PID_SENTINEL.value);
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

				if(*parameter_id == PID_ENDPOINT_GUID.value)
				{
					memcpy(&remote_reader_guid, data+data_index, sizeof(GUID_t));
				}
				else if(*parameter_id == PID_TOPIC_NAME.value)
				{
					topic_name	=  (const char*)(data + data_index + 4);
				}
				else if(*parameter_id == PID_TYPE_NAME.value)
				{
					type_name	=  (const char*)(data + data_index + 4);
				}
				else if(*parameter_id == PID_RELIABILITY.value)
				{
					memcpy(&reliability, data+data_index, sizeof(ReliabilityQosPolicy));
					ByteStream::ChangeByteOrdering(&reliability.kind, sizeof(ReliabilityQosPolicyKind));
					ByteStream::ChangeByteOrdering(&reliability.max_blocking_time.sec, sizeof(long));
					ByteStream::ChangeByteOrdering(&reliability.max_blocking_time.nanosec, sizeof(unsigned long));
				}
				else if(*parameter_id == PID_OWNERSHIP.value)
				{
					memcpy(&ownership, data+data_index, sizeof(OwnershipQosPolicy));
					ByteStream::ChangeByteOrdering(&ownership.kind, sizeof(OwnershipQosPolicyKind));

				}
				else if(*parameter_id == PID_LIVELINESS.value)
				{
					memcpy(&liveliness, data+data_index, sizeof(LivelinessQosPolicy));
					ByteStream::ChangeByteOrdering(&liveliness.kind, sizeof(LivelinessQosPolicyKind));
					ByteStream::ChangeByteOrdering(&liveliness.lease_duration.sec, sizeof(long));
					ByteStream::ChangeByteOrdering(&liveliness.lease_duration.nanosec, sizeof(unsigned long));
				}
				else if(*parameter_id == PID_DURABILITY.value)
				{
					memcpy(&durability, data+data_index, sizeof(DurabilityQosPolicy));
					ByteStream::ChangeByteOrdering(&durability.kind, sizeof(DurabilityQosPolicyKind));
				}
				else if(*parameter_id == PID_DURABILITY_SERVICE.value)
				{
					memcpy(&durability_service, data+data_index, sizeof(DurabilityServiceQosPolicy));
					ByteStream::ChangeByteOrdering(&durability_service.history_depth, sizeof(long));
					ByteStream::ChangeByteOrdering(&durability_service.history_kind, sizeof(HistoryQosPolicyKind));
					ByteStream::ChangeByteOrdering(&durability_service.max_instances, sizeof(long));
					ByteStream::ChangeByteOrdering(&durability_service.max_samples, sizeof(long));
					ByteStream::ChangeByteOrdering(&durability_service.max_samples_per_instance, sizeof(long));
					ByteStream::ChangeByteOrdering(&durability_service.service_cleanup_delay.sec, sizeof(long));
					ByteStream::ChangeByteOrdering(&durability_service.service_cleanup_delay.nanosec, sizeof(unsigned long));
				}
				else if(*parameter_id == PID_PRESENTATION.value)
				{
					memcpy(&presentation, data+data_index, sizeof(PresentationQosPolicy));
					ByteStream::ChangeByteOrdering(&presentation.access_scope, sizeof(PresentationQosPolicyAccessScopeKind));
				}
				else if(*parameter_id == PID_DESTINATION_ORDER.value)
				{
					memcpy(&destination_order, data+data_index, sizeof(DestinationOrderQosPolicy));
					ByteStream::ChangeByteOrdering(&destination_order.kind, sizeof(DestinationOrderQosPolicyKind));
				}
				else if(*parameter_id == PID_DEADLINE.value)
				{
					memcpy(&deadline, data+data_index, sizeof(DeadlineQosPolicy));
					ByteStream::ChangeByteOrdering(&deadline.period.sec, sizeof(long));
					ByteStream::ChangeByteOrdering(&deadline.period.nanosec, sizeof(unsigned long));
				}
				else if(*parameter_id == PID_LATENCY_BUDGET.value)
				{
					memcpy(&latency_budget, data+data_index, sizeof(LatencyBudgetQosPolicy));
					ByteStream::ChangeByteOrdering(&latency_budget.duration.sec, sizeof(long));
					ByteStream::ChangeByteOrdering(&latency_budget.duration.nanosec, sizeof(unsigned long));
				}
				else if(*parameter_id == PID_LIFESPAN.value)
				{
					memcpy(&lifespan, data+data_index, sizeof(LifespanQosPolicy));
					ByteStream::ChangeByteOrdering(&lifespan.duration.sec, sizeof(long));
					ByteStream::ChangeByteOrdering(&lifespan.duration.nanosec, sizeof(unsigned long));
				}

				data_index	+= (short)(*parameter_length);

			}while(*parameter_id != PID_SENTINEL.value);
		}
	}
}
