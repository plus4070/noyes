#include "../../../include/Utility/Timer/TimerEffector.h"
#include "../../../include/Utility/Timer/TimeUnit.h"
#include <stdio.h>

namespace CNU_DDS
{
	TimerEffector::TimerEffector()
	{
		init();
	}

	TimerEffector::~TimerEffector()
	{
		destroy_thread(&thread);
		destroy_mutex(&mutex);
		destroy_condition(&condition);
		delete(list);
	}

	void TimerEffector::init()
	{
		create_mutex(&mutex);
		create_condition(&condition);

		isAlive	= false;
		list	= new ListHeader();
	}

	void TimerEffector::regist(ListHeader* newList)
	{
		mutex_lock(&mutex);
		list->list_add_tail(newList);
		mutex_unlock(&mutex);
		condition_signal(&condition);
	}

	void TimerEffector::start()
	{
		if(!isAlive)
		{
			isAlive	= true;
			create_thread(&thread, running, this);
		}
	}

	void TimerEffector::stop()
	{
		isAlive	= false;
	}

	void TimerEffector::entryTimerEffector()
	{
		mutex_lock(&mutex);
	}

	void TimerEffector::leaveTimerEffector()
	{
		mutex_unlock(&mutex);
	}

	void* TimerEffector::running(void* arg)
	{
		TimerEffector*	effector	= (TimerEffector*)arg;

		effector->entryTimerEffector();

		while(effector->isAlive)
		{
			//effector->entryTimerEffector();
			if(!effector->list->isEmpty())
			{
				TimeUnit*	unit = (TimeUnit*)effector->list->getNext();
				unit->disconnectThis();
				effector->leaveTimerEffector();
				unit->execute(unit->getDataValue());
				delete(unit);
			}
			else
			{
				effector->leaveTimerEffector();
				condition_wait(&effector->condition, &effector->mutex);
				//effector->leaveTimerEffector();
			}
		}

		return 0;
	}
}
