#ifndef LIVELINESSCHANGEDSTATUS_H_
#define LIVELINESSCHANGEDSTATUS_H_

#include "Status.h"
#include "../../DCPS/Infrastructure/Type/InstanceHandle_t.h"

namespace CNU_DDS
{
	class LivelinessChangedStatus:public Status
	{
	public:
		long				alive_count;
		long				not_alive_count;
		long				alive_count_change;
		long				not_alive_count_change;
		InstanceHandle_t	last_publication_handle;
	};
}

#endif
