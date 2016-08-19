#ifndef SAMPLEREJECTEDSTATUS_H_
#define SAMPLEREJECTEDSTATUS_H_

#include "Status.h"
#include "../../DCPS/Infrastructure/Type/InstanceHandle_t.h"

namespace CNU_DDS
{
	enum SampleRejectedStatusKind
	{
		NOT_REJECTED,
		REJECTED_BY_INSTANCES_LIMIT,
		REJECTED_BY_SAMPLES_LIMIT,
		REJECTED_BY_SMAPLES_PER_INSTANCE_LIMIT
	};

	class SampleRejectedStatus:public Status
	{
	public:
		long						total_count;
		long						total_count_change;
		SampleRejectedStatusKind	last_reason;
		InstanceHandle_t			last_instance_handle;
	};
}

#endif
