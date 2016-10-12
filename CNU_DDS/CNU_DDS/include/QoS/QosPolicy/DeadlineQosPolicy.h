#ifndef DEADLINEQOSPOLICY_H_
#define DEADLINEQOSPOLICY_H_

#include "QosPolicy.h"
#include "../../DCPS/Infrastructure/Type/Duration_t.h"

namespace CNU_DDS
{
	class DeadlineQosPolicy: public QosPolicy
	{
	public:
		Duration_t	period;

	public:
		DeadlineQosPolicy(void);
		~DeadlineQosPolicy(void);
	};
}

#endif
