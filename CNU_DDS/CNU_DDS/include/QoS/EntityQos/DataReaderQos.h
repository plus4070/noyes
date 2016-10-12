#ifndef DATAREADERQOS_H_
#define DATAREADERQOS_H_

#include "EntityQos.h"
#include "../QosPolicies.h"

namespace CNU_DDS
{
	class DataReaderQos : public EntityQos
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

#endif
