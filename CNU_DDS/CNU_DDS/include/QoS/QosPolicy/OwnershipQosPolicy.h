#ifndef OWNERSHIPQOSPOLICY_H_
#define OWNERSHIPQOSPOLICY_H_

#include "QosPolicy.h"

namespace CNU_DDS
{
	enum OwnershipQosPolicyKind
	{
		SHARED_OWNERSHIP_QOS,
		EXCLUSIVE_OWNERSHIP_QOS
	};

	class OwnershipQosPolicy : public QosPolicy
	{
	public:
		OwnershipQosPolicyKind	kind;

	public:
		OwnershipQosPolicy(void);
		~OwnershipQosPolicy(void);
	};
}

#endif
