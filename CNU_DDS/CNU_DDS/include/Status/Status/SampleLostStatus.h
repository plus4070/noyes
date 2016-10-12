#ifndef SAMPLELOSTSTATUS_H_
#define SAMPLELOSTSTATUS_H_

#include "Status.h"

namespace CNU_DDS
{
	class SampleLostStatus:public Status
	{
	public:
		long	total_count;
		long	total_count_change;
	};
}

#endif
