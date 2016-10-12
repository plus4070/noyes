#include "TimerTest.h"
#include <iostream>

namespace CNU_DDS
{
	void	TimerTest()
	{
		DynamicTimer*	dt	= DynamicTimer::getDynamicTimerInstance();

		TimeUnit*	tu1	= new TimeUnit(1000);
		TimeUnit*	tu2	= new TimeUnit(5000);

		tu1->setDataValue(1000);
		tu1->setExecuteFunction(TimerTestFunc);

		tu2->setDataValue(2000);
		tu2->setExecuteFunction(TimerTestFunc);

		std::cout << "\nTimerTest Start" << std::endl;

		dt->regist(tu1);
		dt->regist(tu2);

		for(int i=0; i<10; i++)
		{
			Sleep(1000);
			std::cout << "\tcount(" << i << ")" << std::endl;
		}

		std::cout << "\nTimerTest End" << std::endl;
	}

	void	TimerTestFunc(unsigned long arg)
	{
		std::cout << "TimerTestFunc : " << arg << std::endl;
	}
}
