#ifndef TOPICQOS_H_
#define TOPICQOS_H_

#include "EntityQos.h"
#include "../QosPolicies.h"

namespace CNU_DDS
{
	class TopicQos : public EntityQos
	{
	public:
		TopicDataQosPolicy			topic_data;
		DurabilityQosPolicy			durability;
		DurabilityServiceQosPolicy	durability_service;
		DeadlineQosPolicy			deadline;
		LatencyBudgetQosPolicy		latency_budget;
		LivelinessQosPolicy			liveliness;
		ReliabilityQosPolicy		reliability;
		DestinationOrderQosPolicy	destination_order;
		HistoryQosPolicy			history;
		ResourceLimitsQosPolicy		resource_limits;
		TransportPriorityQosPolicy	transport_priority;
		LifespanQosPolicy			lifespan;
		OwnershipQosPolicy			ownership;
	};
}

#endif
