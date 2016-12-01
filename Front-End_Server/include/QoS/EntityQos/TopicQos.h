#ifndef TOPICQOS_H_
#define TOPICQOS_H_

#include "EntityQos.h"
#include "../QosPolicies.h"

#ifdef CNU_DDS_DLL
#define CNU_DDS_DLL_API	__declspec(dllexport)	
#else
#define CNU_DDS_DLL_API	__declspec(dllimport)
#endif
#ifdef __cplusplus
extern "C" {
#endif
namespace CNU_DDS
{
	class CNU_DDS_DLL_API TopicQos : public EntityQos
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

#ifdef __cplusplus
}
#endif
#endif
