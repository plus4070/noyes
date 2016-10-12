#ifndef REQUESTEDINCOMPATIBLEQOSSTATUS_H_
#define REQUESTEDINCOMPATIBLEQOSSTATUS_H_

#include "Status.h"
#include "QosPolicyCount.h"

namespace CNU_DDS
{
	class RequestedIncompatibleQosStatus:public Status
	{
	public:
		long				total_count;
		long				total_count_change;
		QosPolicyId_t		last_policy_id;
		QosPolicyCountSeq	policies;
	};
}

#endif
