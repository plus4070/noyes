#ifndef SUBSCRIPTIONMATCHEDSTATUS_H_
#define SUBSCRIPTIONMATCHEDSTATUS_H_

#include "Status.h"
#include "../../DCPS/Infrastructure/Type/InstanceHandle_t.h"

namespace CNU_DDS
{
	class SubscriptionMatchedStatus:public Status
	{
	public:
		long				total_count;
		long				total_count_change;
		long				current_count;
		long				current_count_change;
		InstanceHandle_t	last_publication_handle;
	};
}

#endif
