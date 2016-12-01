#ifndef DATAWRITERQOS_H_
#define DATAWRITERQOS_H_

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
	class CNU_DDS_DLL_API DataWriterQos : public EntityQos
	{
	public:
		DurabilityQosPolicy				durabiltiy;
		DurabilityServiceQosPolicy		durability_service;
		DeadlineQosPolicy				deadline;
		LatencyBudgetQosPolicy			latency_budget;
		LivelinessQosPolicy				liveliness;
		ReliabilityQosPolicy			reliability;
		DestinationOrderQosPolicy		destination_order;
		HistoryQosPolicy				history;
		ResourceLimitsQosPolicy			resource_limits;
		TransportPriorityQosPolicy		transport_priority;
		LifespanQosPolicy				lifespan;
		UserDataQosPolicy				user_data;
		OwnershipQosPolicy				ownership;
		OwnershipStrengthQosPolicy		ownership_strength;
		WriterDataLifecycleQosPolicy	writer_data_lifecycle;
	};
}

#ifdef __cplusplus
}
#endif
#endif
