#ifndef TIMEBASEDFILTERQOSPOLICY_H_
#define TIMEBASEDFILTERQOSPOLICY_H_

#include "QosPolicy.h"
#include "../../DCPS/Infrastructure/Type/Duration_t.h"

namespace CNU_DDS
{
	class TimeBasedFilterQosPolicy : public QosPolicy
	{
	public:
		Duration_t	minimum_separation;

	public:
		TimeBasedFilterQosPolicy(void);
		~TimeBasedFilterQosPolicy(void);
	};
}

#endif
