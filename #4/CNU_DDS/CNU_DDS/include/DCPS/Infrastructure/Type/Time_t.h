#ifndef TIME_T_H_
#define TIME_T_H_

namespace CNU_DDS
{
	typedef struct
	{
		long			sec;
		unsigned long	nanosec;
	}Time_t;

	const Time_t	TIME_ZERO		= {0, 0};
	const Time_t	TIME_INVALID	= {-1, 0xffffffff};
	const Time_t	TIME_INFINITE	= {0x7fffffff, 0xffffffff};
}

#endif
