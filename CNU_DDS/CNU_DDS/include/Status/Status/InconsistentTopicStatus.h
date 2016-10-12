#ifndef INCONSISTENTTOPICSTATUS_H_
#define INCONSISTENTTOPICSTATUS_H_

#include "Status.h"

namespace CNU_DDS
{
	class InconsistentTopicStatus:public Status
	{
	public:
		long	total_count;
		long	total_count_change;
	};
}

#endif
