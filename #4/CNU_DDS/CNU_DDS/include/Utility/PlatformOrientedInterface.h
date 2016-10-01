#ifndef PLATFORMORIENTEDINTERFACE_H_
#define PLATFORMORIENTEDINTERFACE_H_

#ifdef __CYGWIN__
#undef WIN
#undef _WIN
#undef _WIN32
#undef _WIN64
#undef __WIN__
#endif

#ifdef _WIN32
	#define _AFXDLL
	#undef _WINDOWS_
#endif

#ifdef _WIN32
#include <process.h>
#include <afxmt.h>
#include <signal.h>
#include <stdlib.h>
#include <memory>
#else
#include <pthread.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>
#include <synchapi.h>
#include <stdlib.h>
#include <memory>
#endif

namespace CNU_DDS
{
	#define	DOMAINID_TYPE_NATIVE			long
	#define	HANDLE_TYPE_NATIVE				long
	#define	HANDLE_NIL_NATIVE				0
	#define	BUILTIN_TOPIC_KEY_TYPE_NATIVE	long

#ifdef _WIN32
	#define THREAD_TYPE_NATIVE				HANDLE
	#define	MUTEX_TYPE_NATIVE				CRITICAL_SECTION
	#define CONDITION_TYPE_NATIVE			HANDLE
	#define TIME_TYPE_NATIVE				time_t
	#define SHARED_PTR_TYPE_NATIVE			std::shared_ptr

	#define	CLOCK_ID_DDS_TIME				(clockid_t)32
#else
	#define THREAD_TYPE_NATIVE				pthread_t
	#define	MUTEX_TYPE_NATIVE				pthread_mutex_t
	#define CONDITION_TYPE_NATIVE			pthread_cond_t
	#define TIME_TYPE_NATIVE				time_t
	#define SHARED_PTR_TYPE_NATIVE			std::shared_ptr

	#define	CLOCK_ID_DDS_TIME				(clockid_t)32
#endif
	int	create_thread(THREAD_TYPE_NATIVE* thread, void* (*start_routine)(void *), void* arg);
	int	create_mutex(MUTEX_TYPE_NATIVE* mutex);
	int	create_condition(CONDITION_TYPE_NATIVE* condition);

	int	destroy_thread(THREAD_TYPE_NATIVE* thread);
	int	destroy_mutex(MUTEX_TYPE_NATIVE* mutex);
	int	destroy_condition(CONDITION_TYPE_NATIVE* condition);

	int	condition_wait(CONDITION_TYPE_NATIVE* condition, MUTEX_TYPE_NATIVE* mutex);
	int	condition_signal(CONDITION_TYPE_NATIVE* condition);

	int	mutex_lock(MUTEX_TYPE_NATIVE* mutex);
	int mutex_unlock(MUTEX_TYPE_NATIVE* mutex);

	void			native_sleep(unsigned long millisecond);
	void			start_local_time();
	unsigned long	get_current_time();

	TIME_TYPE_NATIVE	get_system_time();
}

#endif
