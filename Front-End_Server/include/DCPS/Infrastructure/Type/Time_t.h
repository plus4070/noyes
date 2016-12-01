#ifndef TIME_T_H_
#define TIME_T_H_

#ifdef CNU_DDS_DLL
#define CNU_DDS_DLL_API	__declspec(dllexport)	
#else
#define CNU_DDS_DLL_API	__declspec(dllimport)
#endif
#ifdef __cplusplus
extern "C" {
#endif
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

#ifdef __cplusplus
}
#endif
#endif
