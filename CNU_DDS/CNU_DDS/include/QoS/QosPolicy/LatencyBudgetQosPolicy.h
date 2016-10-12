#ifndef LATENCYBUDGETQOSPOLICY_H_
#define LATENCYBUDGETQOSPOLICY_H_

#include "QosPolicy.h"
#include "../../DCPS/Infrastructure/Type/Duration_t.h"

namespace CNU_DDS
{
	class LatencyBudgetQosPolicy : public QosPolicy
	{
	public:
		Duration_t	duration;

	public:
		LatencyBudgetQosPolicy(void);
		~LatencyBudgetQosPolicy(void);
	};
}

#endif
