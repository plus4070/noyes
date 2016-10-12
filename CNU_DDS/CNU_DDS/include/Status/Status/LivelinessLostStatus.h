#ifndef LIVELINESSLOSTSTATUS_H_
#define LIVELINESSLOSTSTATUS_H_

#include "Status.h"

namespace CNU_DDS
{
	class LivelinessLostStatus:public Status
	{
	public:
		long	total_count;
		long	total_count_change;
	};
}

#endif
