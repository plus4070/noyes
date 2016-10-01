#ifndef OFFEREDDEADLINEMISSEDSTATUS_H_
#define OFFEREDDEADLINEMISSEDSTATUS_H_

#include "Status.h"
#include "../../DCPS/Infrastructure/Type/InstanceHandle_t.h"

namespace CNU_DDS
{
	class OfferedDeadlineMissedStatus:public Status
	{
	public:
		long				total_count;
		long				total_count_change;
		InstanceHandle_t	last_instance_handle;
	};
}

#endif
