#ifndef TOPICBUILTINTOPICDATA_H_
#define TOPICBUILTINTOPICDATA_H_

#include "../QoS/QosPolicies.h"
#include "Type/BuiltinTopicKey_t.h"
#include "../DCPS/Topic/DDS_Data.h"

namespace CNU_DDS
{
	class TopicBuiltinTopicData: public DDS_Data
	{
	public:	// It has been defined as private in a Discovery Module in RTPS Specification.
		BuiltinTopicKey_t			key;
		std::string					topic_name;
		std::string					type_name;

		DurabilityQosPolicy			durability;
		DurabilityServiceQosPolicy	durability_service;
		DeadlineQosPolicy			deadline;
		LatencyBudgetQosPolicy		latency_budget;
		LivelinessQosPolicy			liveliness;
		ReliabilityQosPolicy		reliability;
		TransportPriorityQosPolicy	transport_priority;
		LifespanQosPolicy			lifespan;
		DestinationOrderQosPolicy	destination_order;
		HistoryQosPolicy			history;
		ResourceLimitsQosPolicy		resource_limits;
		OwnershipQosPolicy			ownership;
		TopicDataQosPolicy			topic_data;

	public:
		virtual ByteStream*	Serialize()							= 0;
		virtual void		Deserialize(ByteStream* stream)		= 0;
	};
}

#endif
