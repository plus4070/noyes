#ifndef RELIABILITYQOSPOLICY_H_
#define RELIABILITYQOSPOLICY_H_

#include "QosPolicy.h"
#include "../../DCPS/Infrastructure/Type/Duration_t.h"

namespace CNU_DDS
{
	enum ReliabilityQosPolicyKind
	{
		BEST_EFFROT_REIABILITY_QOS	= 1,
		RELIABLE_RELIABILITY_QOS	= 2
	};

	class ReliabilityQosPolicy : public QosPolicy
	{
	public:
		ReliabilityQosPolicyKind	kind;
		Duration_t					max_blocking_time;

	public:
		ReliabilityQosPolicy(void);
		~ReliabilityQosPolicy(void);
	};
}

#endif
