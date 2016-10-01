#ifndef LIFESPANQOSPOLICY_H_
#define LIFESPANQOSPOLICY_H_

#include "QosPolicy.h"
#include "../../DCPS/Infrastructure/Type/Duration_t.h"

namespace CNU_DDS
{
	class LifespanQosPolicy : public QosPolicy
	{
	public:
		Duration_t	duration;

	public:
		LifespanQosPolicy(void);
		~LifespanQosPolicy(void);
	};
}

#endif
