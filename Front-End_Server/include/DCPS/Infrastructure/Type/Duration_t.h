#ifndef DURATION_T_H_
#define DURATION_T_H_

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
	class CNU_DDS_DLL_API Duration_t
	{
	public:
		long			sec;
		unsigned long	nanosec;
	public:
		Duration_t();
		Duration_t(long a_sec, unsigned long a_nanosec);
		~Duration_t();

		unsigned long		getDurationByMilliSecond();

		void	operator	= (const Duration_t& arg);
		void	operator	+=(const Duration_t& arg);
	};

	const	long			DURATION_INFINITE_SEC	= 0x7FFFFFFF;
	const	unsigned long	DURATION_INFINITE_NSEC	= 0x7FFFFFFF;

	const	long			DURATION_ZERO_SEC		= 0x00000000;
	const	unsigned long	DURATION_ZERO_NSEC		= 0x00000000;

	const	long			TIME_INVALID_SEC		= -1;
	const	unsigned long	TIME_INVALID_NSEC		= 0xFFFFFFFF;
}

#ifdef __cplusplus
}
#endif
#endif
