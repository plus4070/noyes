#include "../../include/Utility/PlatformOrientedInterface.h"

namespace CNU_DDS
{

	int	create_thread(THREAD_TYPE_NATIVE* thread, void* (*start_routine)(void *), void* arg)
	{
		int result	= 0;
		
#ifdef _WIN32
		result		= _beginthread((void (*)(void*))start_routine, 0, arg);
#else
		result		= pthread_create(thread, 0, start_routine, arg);
#endif

		return result;
	}

	int	create_mutex(MUTEX_TYPE_NATIVE* mutex)
	{
		int result	= 0;

#ifdef _WIN32
		InitializeCriticalSection(mutex);
#else
		result	= pthread_mutex_init(mutex, NULL);
#endif

		return result;
	}

	int	create_condition(CONDITION_TYPE_NATIVE* condition)
	{
		int result	= 0;

#ifdef _WIN32
		*condition	= CreateEvent(NULL, FALSE, FALSE, NULL);
#else
		result	= pthread_cond_init(condition, NULL);
#endif

		return result;
	}

	int	destroy_thread(THREAD_TYPE_NATIVE* thread)
	{
		int result	= 0;

#ifdef _WIN32
		CloseHandle(*thread);
#else
		result	= destroy_thread(thread);
#endif

		return result;
	}

	int	destroy_mutex(MUTEX_TYPE_NATIVE* mutex)
	{
		int result	= 0;

#ifdef _WIN32
		DeleteCriticalSection(mutex);
#else
		result	= pthread_mutex_destroy(mutex);
#endif

		return result;
	}

	int	destroy_condition(CONDITION_TYPE_NATIVE* condition)
	{
		int result	= 0;

#ifdef _WIN32
		CloseHandle(*condition);
#else
		result	= pthread_cond_destroy(condition);
#endif

		return result;
	}

	int	condition_wait(CONDITION_TYPE_NATIVE* condition, MUTEX_TYPE_NATIVE* mutex)
	{
		int result	= 0;

#ifdef _WIN32
		WaitForSingleObject(*condition, INFINITE);
#else
		result	= pthread_cond_wait(condition, mutex);
#endif

		return result;
	}

	int	condition_signal(CONDITION_TYPE_NATIVE* condition)
	{
		int result	= 0;

#ifdef _WIN32
		SetEvent(*condition);
#else
		result	= pthread_cond_signal(condition);
#endif

		return result;
	}

	int	mutex_lock(MUTEX_TYPE_NATIVE* mutex)
	{
		int	result	= 0;

#ifdef _WIN32
		EnterCriticalSection(mutex);
#else
		result	= pthread_mutex_lock(mutex);
#endif

		return result;
	}

	int mutex_unlock(MUTEX_TYPE_NATIVE* mutex)
	{
		int	result	= 0;

#ifdef _WIN32
		LeaveCriticalSection(mutex);
#else
		result	= pthread_mutex_unlock(mutex);
#endif

		return result;
	}

	TIME_TYPE_NATIVE	get_system_time()
	{
		TIME_TYPE_NATIVE	cur_time;

#ifdef _WIN32
		cur_time	= time(0);
#else
		time(&cur_time);
#endif

		return cur_time;
	}

	void	native_sleep(unsigned long millisecond)
	{
#ifdef _WIN32
		Sleep(millisecond);
#else
		Sleep(millisecond);
#endif
	}

#ifdef _WIN32
#else
	bool				LOCAL_TIME_IS_STARTED;
	struct	timespec	LOCAL_TIME;
#endif

	void			start_local_time()
	{
#ifdef _WIN32
#else

		if(LOCAL_TIME_IS_STARTED == false)
		{
			//clock_gettime(CLOCK_REALTIME, &LOCAL_TIME);
			clock_gettime(CLOCK_MONOTONIC, &LOCAL_TIME);
			LOCAL_TIME_IS_STARTED	= true;
		}
#endif
	}

	unsigned long		get_current_time()
	{
		unsigned long	cur_time	= 0;

#ifdef _WIN32
		cur_time	= clock();
#else
		//cur_time	= (unsigned long)clock();//*1000/CLOCKS_PER_SEC;
		if(!LOCAL_TIME_IS_STARTED)
		{
			start_local_time();
		}

		struct timespec	ts;
		//clock_gettime(CLOCK_REALTIME, &ts);
		clock_gettime(CLOCK_MONOTONIC, &ts);

		cur_time	= (ts.tv_sec - LOCAL_TIME.tv_sec)*1000;
		cur_time	+= (ts.tv_nsec - LOCAL_TIME.tv_nsec)/(1000*1000);
#endif

		return cur_time;
	}
}
