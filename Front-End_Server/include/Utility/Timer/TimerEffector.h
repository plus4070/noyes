#ifndef INCLUDE_UTILITY_TIMER_TIMEREFFECTOR_H_
#define INCLUDE_UTILITY_TIMER_TIMEREFFECTOR_H_

#include "../PlatformOrientedInterface.h"
#include "ListHeader.h"

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
	class CNU_DDS_DLL_API TimerEffector
	{
	private:
		MUTEX_TYPE_NATIVE		mutex;
		CONDITION_TYPE_NATIVE	condition;
		THREAD_TYPE_NATIVE		thread;

		bool					isAlive;
		ListHeader*				list;

	private:
		void			init();
		static void*	running(void* arg);

	public:
		TimerEffector();
		~TimerEffector();

		void		regist(ListHeader* newList);
		void		start();
		void		stop();

		void		entryTimerEffector();
		void		leaveTimerEffector();
	};
}

#ifdef __cplusplus
}
#endif
#endif /* INCLUDE_UTILITY_TIMER_TIMEREFFECTOR_H_ */
