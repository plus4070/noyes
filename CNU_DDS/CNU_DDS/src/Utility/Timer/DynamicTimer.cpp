#include "../../../include/Utility/Timer/DynamicTimer.h"
#include <stdio.h>

namespace CNU_DDS
{
	const int DynamicTimer::TVN_BITS	= 6;
	const int DynamicTimer::TVR_BITS	= 8;
	const int DynamicTimer::TVN_SIZE	= 64;//1 << TVN_BITS;
	const int DynamicTimer::TVR_SIZE	= 256;//1 << TVR_SIZE;
	const int DynamicTimer::TVN_MASK	= 63;//TVN_SIZE - 1;
	const int DynamicTimer::TVR_MASK	= 255;//TVR_SIZE - 1;

	DynamicTimer::DynamicTimer(Duration_t a_Interval, TimerEffector* a_Effector)
	{
		interval	= a_Interval;
		printf("DynamicTimer->interval(%d)\n", interval.getDurationByMilliSecond());
		effector	= a_Effector;
		init();
	}

	DynamicTimer::~DynamicTimer()
	{
		destroy_thread(&thread);
		destroy_mutex(&mutex);
	}

	DynamicTimer*	DynamicTimer::getDynamicTimerInstance()
	{
		if(DYNAMIC_TIMER_INSTANCE == NULL)
		{
			TIMER_EFFECTOR_INSTANCE	= new TimerEffector();
			//DYNAMIC_TIMER_INSTANCE	= new DynamicTimer(Duration_t(1, 0), TIMER_EFFECTOR_INSTANCE);
			DYNAMIC_TIMER_INSTANCE	= new DynamicTimer(Duration_t(0, 100*1000*1000), TIMER_EFFECTOR_INSTANCE);

			TIMER_EFFECTOR_INSTANCE->start();
			DYNAMIC_TIMER_INSTANCE->start();
		}

		return DYNAMIC_TIMER_INSTANCE;
	}

	void DynamicTimer::init()
	{
		create_mutex(&mutex);

		tv1				= new ListHeader[256];
		tv2				= new ListHeader[256];
		tv3				= new ListHeader[256];
		tv4				= new ListHeader[256];
		tv5				= new ListHeader[256];

		jiffies.sec		= 0;
		jiffies.nanosec	= 0;
		isAlive			= false;
		runningTimer	= NULL;
	}

	Duration_t DynamicTimer::getCurrentJiffies()
	{
		return jiffies;
	}

	int DynamicTimer::getCalcIndex(int level)
	{
		int calcIndex = (((jiffies.getDurationByMilliSecond()/interval.getDurationByMilliSecond()) >> (TVR_BITS + (level) * TVN_BITS)) & TVN_MASK);
		return calcIndex;
	}

	int DynamicTimer::cascade(ListHeader* tv, int index)
	{
		ListHeader	tv_list;
		ListHeader*	timer;
		ListHeader*	tmp;

		ListHeader::listReplaceInit(tv + index, &tv_list);
		timer = tv_list.getNext();

		while(timer != &tv_list)
		{
			tmp		= timer->getNext();

			regist((TimeUnit*)timer);

			timer	= tmp;
		}

		return index;
	}

	unsigned long DynamicTimer::getCalcInterval()
	{
		unsigned long result;

		result	= interval.sec*1000;
		result	+= interval.nanosec/(1000*1000);

		return result;
	}

	void DynamicTimer::regist(TimeUnit* timer)
	{
		//Linux의 DynamicTimer의 경우 Tick의 단위가 1이기 때문에 현재와 같은 수식을 사용
		//우리가 사용할 DynamicTimer의 경우 Tick의 단위가 1이 아니기 때문에 수정이 필요함
		entryDynamicTimer();

		//unsigned long	expires = timer->getExpireTime(); //expiretime이 현재 시간에서 계산되어 들어오는 경우
		//unsigned long	idx = (expires - jiffies.getDurationByMilliSecond())/interval.getDurationByMilliSecond();
		unsigned long	expires = timer->getExpireTime() + (jiffies.getDurationByMilliSecond());
		unsigned long	idx = (expires - jiffies.getDurationByMilliSecond())/interval.getDurationByMilliSecond();
		ListHeader*		vec;

		if (idx < TVR_SIZE)
		{
			int i = (expires/interval.getDurationByMilliSecond()) & TVR_MASK;
			vec = tv1 + i;
		}
		else if (idx < 1 << (TVR_BITS + TVN_BITS))
		{
			int i = ((expires/interval.getDurationByMilliSecond()) >> TVR_BITS) & TVN_MASK;
			vec = tv2 + i;
		}
		else if (idx < 1 << (TVR_BITS + 2 * TVN_BITS))
		{
			int i = ((expires/interval.getDurationByMilliSecond()) >> (TVR_BITS + TVN_BITS)) & TVN_MASK;
			vec = tv3 + i;
		}
		else if (idx < 1 << (TVR_BITS + 3 * TVN_BITS))
		{
			int i = ((expires/interval.getDurationByMilliSecond()) >> (TVR_BITS + 2 * TVN_BITS)) & TVN_MASK;
			vec = tv4 + i;
		}
		else if ((signed long) idx < 0)
		{
			vec = tv1 + (jiffies.getDurationByMilliSecond()/interval.getDurationByMilliSecond() & TVR_MASK);
		}
		else
		{
			int i;

			if (idx > 0xffffffffUL)
			{
				idx = 0xffffffffUL;
				expires = idx + (jiffies.getDurationByMilliSecond()/interval.getDurationByMilliSecond());
			}
			i = ((expires/interval.getDurationByMilliSecond()) >> (TVR_BITS + 3 * TVN_BITS)) & TVN_MASK;
			vec = tv5 + i;
		}

		vec->list_add_tail(timer);

		leaveDynamicTimer();
	}

	void DynamicTimer::start()
	{
		if(isAlive)
		{
			return;
		}
		else
		{
			isAlive	= true;
			create_thread(&thread, running, this);
		}
	}

	void DynamicTimer::stop()
	{
		isAlive	= false;
	}

	void DynamicTimer::entryDynamicTimer()
	{
		mutex_lock(&mutex);
	}

	void DynamicTimer::leaveDynamicTimer()
	{
		mutex_unlock(&mutex);
	}

	void* DynamicTimer::running(void* arg)
	{
		DynamicTimer*	timer	= (DynamicTimer*)arg;
		//int				index	= 0;
		int				index	= 1;

		while(timer->isAlive)
		{
			//index	= (timer->jiffies.getDurationByMilliSecond()/timer->interval.getDurationByMilliSecond()) & DynamicTimer.TVR_MASK;
			timer->jiffies	+= timer->interval;
			int tmp		= timer->jiffies.getDurationByMilliSecond()/timer->interval.getDurationByMilliSecond();
			int tmp2	= DynamicTimer::TVR_MASK;
			index	= tmp & tmp2;

			if(	!index && (!timer->cascade(timer->tv2, timer->getCalcIndex(0))) &&	(!timer->cascade(timer->tv3, timer->getCalcIndex(1))) && (!timer->cascade(timer->tv4, timer->getCalcIndex(2))))
			{
				timer->cascade(timer->tv5, timer->getCalcIndex(3));
			}

			//실행 전용 thread에 timer list를 넘김
			if(!timer->tv1[index].isEmpty())
			{
				ListHeader	newList;
				ListHeader::listReplaceInit(timer->tv1 + index, &newList);
				timer->effector->regist(newList.disconnectMeaningfulList());
			}

			native_sleep(timer->interval.getDurationByMilliSecond());
		}

		return 0;
	}

	void DynamicTimer::SET_TEST_JIFFIES(Duration_t a_jiffies)
	{
		jiffies	= a_jiffies;
	}
}
