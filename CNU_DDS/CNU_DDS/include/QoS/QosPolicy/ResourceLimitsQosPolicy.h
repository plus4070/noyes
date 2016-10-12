#ifndef RESOURCELIMITSQOSPOLICY_H_
#define RESOURCELIMITSQOSPOLICY_H_

#include "QosPolicy.h"

namespace CNU_DDS
{
	class ResourceLimitsQosPolicy : public QosPolicy
	{
	public:
		long	max_samples;
		long	max_instances;
		long	max_samples_per_instance;

	public:
		ResourceLimitsQosPolicy(void);
		~ResourceLimitsQosPolicy(void);
	};
}

#endif
