#ifndef DURABILITYQOSPOLICY_H_
#define DURABILITYQOSPOLICY_H_

#include "QosPolicy.h"

namespace CNU_DDS
{
	enum DurabilityQosPolicyKind
	{
		VOLATILE_DURABILITY_QOS,
		TRANSIENT_LOCAL_DURABILITY_QOS,
		TRANSIENT_DURABILITY_QOS,
		PERSISTENT_DURABILITY_QOS
	};

	class DurabilityQosPolicy : public QosPolicy
	{
	public:
		DurabilityQosPolicyKind	kind;

	public:
		DurabilityQosPolicy(void);
		~DurabilityQosPolicy(void);
	};
}

#endif
