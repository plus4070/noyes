#ifndef INCLUDE_UTILITY_TIMER_TIMEREFFECTOR_H_
#define INCLUDE_UTILITY_TIMER_TIMEREFFECTOR_H_

#include "../PlatformOrientedInterface.h"
#include "ListHeader.h"

namespace CNU_DDS
{
	class TimerEffector
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

#endif /* INCLUDE_UTILITY_TIMER_TIMEREFFECTOR_H_ */
