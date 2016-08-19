#ifndef OWNERSHIPSTRENGTHQOSPOLICY_H_
#define OWNERSHIPSTRENGTHQOSPOLICY_H_

#include "QosPolicy.h"

namespace CNU_DDS
{
	class OwnershipStrengthQosPolicy : public QosPolicy
	{
	public:
		long	value;

	public:
		OwnershipStrengthQosPolicy(void);
		~OwnershipStrengthQosPolicy(void);
	};
}

#endif
