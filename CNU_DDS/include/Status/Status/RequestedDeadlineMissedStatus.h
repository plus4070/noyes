#ifndef REQUESTEDDEADLINEMISSEDSTATUS_H_
#define REQUESTEDDEADLINEMISSEDSTATUS_H_

#include "Status.h"
#include "../../DCPS/Infrastructure/Type/InstanceHandle_t.h"

namespace CNU_DDS
{
	class RequestedDeadlineMissedStatus:public Status
	{
	public:
		long				total_count;
		long				total_count_change;
		InstanceHandle_t	last_instance_handle;
	};
}

#endif
