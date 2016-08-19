#ifndef DURABILITYSERVICEQOSPOLICY_H_
#define DURABILITYSERVICEQOSPOLICY_H_

#include "QosPolicy.h"
#include "HistoryQosPolicy.h"
#include "../../DCPS/Infrastructure/Type/Duration_t.h"

namespace CNU_DDS
{
	class DurabilityServiceQosPolicy : public QosPolicy
	{
	public:
		Duration_t				service_cleanup_delay;
		HistoryQosPolicyKind	history_kind;
		long					history_depth;
		long					max_samples;
		long					max_instances;
		long					max_samples_per_instance;

	public:
		DurabilityServiceQosPolicy(void);
		~DurabilityServiceQosPolicy(void);
	};
}

#endif
