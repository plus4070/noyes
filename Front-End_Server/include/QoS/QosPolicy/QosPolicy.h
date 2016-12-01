#ifndef QOSPOLICY_H_
#define QOSPOLICY_H_

#include <iostream>

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
	class CNU_DDS_DLL_API QosPolicy
	{
/*
	protected:
		std::string	name;
*/
	public:
		QosPolicy();
		~QosPolicy();
	};

	const std::string	USER_DATA_QOS_POLICY_NAME				= "UserData";
	const std::string	DURABILITY_QOS_POLICY_NAME				= "Durability";
	const std::string	PRESENTATION_QOS_POLICY_NAME			= "Presentation";
	const std::string	DEADLINE_QOS_POLICY_NAME				= "Deadline";
	const std::string	LATENCY_BUDGET_QOS_POLICY_NAME			= "LatencyBudget";

	const std::string	OWNERSHIP_QOS_POLICY_NAME				= "Ownership";
	const std::string	OWNERSHIP_STRENGTH_QOS_POLICY_NAME		= "OwnershipStrength";
	const std::string	LIVELINESS_QOS_POLICY_NAME				= "Liveliness";
	const std::string	TIME_BASED_FILTER_QOS_POLICY_NAME		= "TimeBasedFilter";
	const std::string	PARTITION_QOS_POLICY_NAME				= "Partition";

	const std::string	RELIABILITY_QOS_POLICY_NAME				= "Reliability";
	const std::string	DESTINAITON_ORDER_QOS_POLICY_NAME		= "DestinationOrder";
	const std::string	HISTORY_QOS_POLICY_NAME					= "History";
	const std::string	RESOURCE_LIMITS_QOS_POLICY_NAME			= "ResourceLimits";
	const std::string	ENTITY_FACTORY_QOS_POLICY_NAME			= "EntityFactory";

	const std::string	WRITER_DATA_LIFECYCLE_QOS_POLICY_NAME	= "WriterDataLifecycle";
	const std::string	READER_DATA_LIFECYCLE_QOS_POLICY_NAME	= "ReaderDataLifecycle";
	const std::string	TOPIC_DATA_QOS_POLICY_NAME				= "TopicData";
	const std::string	GROUP_DATA_QOS_POLICY_NAME				= "GroupData";
	const std::string	TRANSPORT_PRIORITY_QOS_POLICY_NAME		= "TransportPriority";

	const std::string	LIFESPAN_QOS_POLICY_NAME				= "Lifespan";
	const std::string	DURABILITY_SERVICE_QOS_POLICY_NAME		= "DurabilityService";
}

#ifdef __cplusplus
}
#endif
#endif
