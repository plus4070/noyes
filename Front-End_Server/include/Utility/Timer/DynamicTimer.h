#ifndef INCLUDE_UTILITY_TIMER_DYNAMICTIMER_H_
#define INCLUDE_UTILITY_TIMER_DYNAMICTIMER_H_

#include "../PlatformOrientedInterface.h"
#include "TimeUnit.h"
#include "TimerEffector.h"
#include "../../RTPS/Structure/Type/Duration_t.h"

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
	class CNU_DDS_DLL_API DynamicTimer
	{
	private:
		THREAD_TYPE_NATIVE	thread;
		MUTEX_TYPE_NATIVE	mutex;

		Duration_t			interval;
		Duration_t			jiffies;
		bool				isAlive;
		TimeUnit*			runningTimer;
		TimerEffector*		effector;

		ListHeader*			tv1;
		ListHeader*			tv2;
		ListHeader*			tv3;
		ListHeader*			tv4;
		ListHeader*			tv5;

	private:
		static const int	TVN_BITS;
		static const int	TVR_BITS;
		static const int	TVN_SIZE;
		static const int	TVR_SIZE;
		static const int	TVN_MASK;
		static const int	TVR_MASK;

	private:
		void				init();
		static void*		running(void* arg);

		int					getCalcIndex(int level);
		int					cascade(ListHeader* tv, int index);

	private:
		DynamicTimer(Duration_t a_Interval, TimerEffector* a_Effector);
		~DynamicTimer();

	public:
		static DynamicTimer*	getDynamicTimerInstance();

		Duration_t			getCurrentJiffies();
		unsigned long		getCalcInterval();

		void				regist(TimeUnit* timer);
		void				start();
		void				stop();

		void				entryDynamicTimer();
		void				leaveDynamicTimer();

	public:
		void				SET_TEST_JIFFIES(Duration_t a_jiffies);
	};

	static	DynamicTimer*	DYNAMIC_TIMER_INSTANCE;
	static	TimerEffector*	TIMER_EFFECTOR_INSTANCE;
}

#ifdef __cplusplus
}
#endif
#endif /* INCLUDE_UTILITY_TIMER_DYNAMICTIMER_H_ */
