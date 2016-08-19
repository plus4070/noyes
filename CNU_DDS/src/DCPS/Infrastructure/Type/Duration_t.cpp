#include "../../../../include/DCPS/Infrastructure/Type/Duration_t.h"

namespace CNU_DDS
{
	Duration_t::Duration_t()
	{
		sec		= 0;
		nanosec	=0;
	}

	Duration_t::Duration_t(long a_sec, unsigned long a_nanosec)
	{
		nanosec	= a_nanosec;
		sec		= a_sec + (nanosec / (1000 * 1000 * 1000));

		nanosec	%=	(1000*1000*1000);
	}

	Duration_t::~Duration_t()
	{

	}

	unsigned long		Duration_t::getDurationByMilliSecond()
	{
		return (sec*1000)+(nanosec/(1000*1000));
	}

	void	Duration_t::operator	= (const Duration_t& arg)
	{
		sec		= arg.sec;
		nanosec	= arg.nanosec;
	}

	void	Duration_t::operator	+=(const Duration_t& arg)
	{
		nanosec	+= arg.nanosec;
		sec		+= arg.sec + (nanosec / (1000 * 1000 * 1000));

		nanosec	%=	(1000*1000*1000);
	}
}
