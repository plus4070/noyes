#ifndef TRANSPORTPRIORITYQOSPOLICY_H_
#define TRANSPORTPRIORITYQOSPOLICY_H_

#include "QosPolicy.h"

namespace CNU_DDS
{
	class TransportPriorityQosPolicy : public QosPolicy
	{
	public:
		long	value;

	public:
		TransportPriorityQosPolicy(void);
		~TransportPriorityQosPolicy(void);
	};
}

#endif
