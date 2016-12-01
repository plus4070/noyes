#ifndef DATAREADERQOS_H_
#define DATAREADERQOS_H_

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
	class CNU_DDS_DLL_API DataReaderQos : public EntityQos
	{
	public:
		DurabilityQosPolicy				durability;
		DeadlineQosPolicy				deadline;
		LatencyBudgetQosPolicy			latency_budget;
		LivelinessQosPolicy				liveliness;
		ReliabilityQosPolicy			reliability;
		DestinationOrderQosPolicy		destination_order;
		HistoryQosPolicy				history;
		ResourceLimitsQosPolicy			resource_limits;
		UserDataQosPolicy				user_data;
		OwnershipQosPolicy				ownership;
		TimeBasedFilterQosPolicy		time_based_filter;
		ReaderDataLifecycleQosPolicy	reader_data_lifecycle;
	};
}

#ifdef __cplusplus
}
#endif
#endif
