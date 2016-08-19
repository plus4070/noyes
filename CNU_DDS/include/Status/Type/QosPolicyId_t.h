#ifndef QOSPOLICYID_T_H_
#define QOSPOLICYID_T_H_

namespace CNU_DDS
{
	typedef long	QosPolicyId_t;

	const QosPolicyId_t	INVALID_QOS_POLICY_ID				= 0;
	const QosPolicyId_t	USER_DATA_QOS_POLICY_ID				= 1;
	const QosPolicyId_t	DURABILITY_QOS_POLICY_ID			= 2;
	const QosPolicyId_t	PRESENTATION_QOS_POLICY_ID			= 3;
	const QosPolicyId_t	DEADLINE_QOS_POLICY_ID				= 4;

	const QosPolicyId_t	LATENCY_BUDGET_QOS_POLICY_ID		= 5;
	const QosPolicyId_t	OWNERSHIP_QOS_POLICY_ID				= 6;
	const QosPolicyId_t	OWNERSHIP_STRENGTH_QOS_POLICY_ID	= 7;
	const QosPolicyId_t	LIVELINESS_QOS_POLICY_ID			= 8;
	const QosPolicyId_t	TIME_BASED_FILTER_QOS_POLICY_ID		= 9;

	const QosPolicyId_t	PARTITION_QOS_POLICY_ID				= 10;
	const QosPolicyId_t	RELIABILITY_QOS_POLICY_ID			= 11;
	const QosPolicyId_t	DESTINATION_ORDER_QOS_POLICY_ID		= 12;
	const QosPolicyId_t	HISTORY_QOS_POLICY_ID				= 13;
	const QosPolicyId_t	RESOURCE_LIMITS_QOS_POLICY_ID		= 14;

	const QosPolicyId_t	ENTITY_FACTORY_QOS_POLICY_ID		= 15;
	const QosPolicyId_t	WRITER_DATA_LIFECYCLE_QOS_POLICY_ID	= 16;
	const QosPolicyId_t	READER_DATA_LIFECYCLE_QOS_POLICY_ID	= 17;
	const QosPolicyId_t	TOPIC_DATA_QOS_POLICY_ID			= 18;
	const QosPolicyId_t	GROUP_DATA_QOS_POLICY_ID			= 19;

	const QosPolicyId_t	TRANSPORT_PRIORITY_QOS_POLICY_ID	= 20;
	const QosPolicyId_t	LIFESPAN_QOS_POLICY_ID				= 21;
	const QosPolicyId_t	DURABILITY_SERVICE_QOS_POLICY_ID	= 22;
}

#endif
