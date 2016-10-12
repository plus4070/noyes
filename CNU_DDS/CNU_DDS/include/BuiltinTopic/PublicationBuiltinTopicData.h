#ifndef PUBLICATIONBUILTINTOPICDATA_H_
#define PUBLICATIONBUILTINTOPICDATA_H_

#include "../QoS/QosPolicies.h"
#include "Type/BuiltinTopicKey_t.h"
#include "../DCPS/Topic/DDS_Data.h"

namespace CNU_DDS
{
	class PublicationBuiltinToipcData: public DDS_Data
	{
	public:	// It has been defined as private in a Discovery Module in RTPS Specification.
		BuiltinTopicKey_t			key;
		BuiltinTopicKey_t			participant_key;
		std::string					topic_name;
		std::string					type_name;

		DurabilityQosPolicy			durability;
		DurabilityServiceQosPolicy	durability_service;
		DeadlineQosPolicy			deadline;
		LatencyBudgetQosPolicy		latency_budget;
		LivelinessQosPolicy			liveliness;
		ReliabilityQosPolicy		reliability;
		LifespanQosPolicy			lifespan;
		UserDataQosPolicy			user_data;
		OwnershipQosPolicy			ownership;
		OwnershipStrengthQosPolicy	ownership_strength;
		DestinationOrderQosPolicy	destination_order;
		PresentationQosPolicy		presentation;
		PartitionQosPolicy			partition;
		TopicDataQosPolicy			topic_data;
		GroupDataQosPolicy			group_data;

	public:
		virtual ByteStream*	Serialize()							= 0;
		virtual void		Deserialize(ByteStream* stream)		= 0;
	};
}

#endif
