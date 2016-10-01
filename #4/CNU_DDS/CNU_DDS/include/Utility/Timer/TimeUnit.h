#ifndef INCLUDE_UTILITY_TIMER_TIMEUNIT_H_
#define INCLUDE_UTILITY_TIMER_TIMEUNIT_H_

#include "ListHeader.h"
#include "../PlatformOrientedInterface.h"

namespace CNU_DDS
{
	class TimeUnit	: public ListHeader
	{
	private:
		void				(*function)(unsigned long);
		unsigned long		expires;
		unsigned long		data;
		MUTEX_TYPE_NATIVE	mutex;

	public:
		TimeUnit(unsigned long a_Expire);
		~TimeUnit();

		void				setExpireTime(unsigned long a_Expire);
		void				setDataValue(unsigned long a_Value);
		void				setExecuteFunction(void (*a_function)(unsigned long));

		unsigned long		getExpireTime();
		unsigned long		getDataValue();

		void				execute(unsigned long arg);

		void				entryCriticalSection();
		void				leaveCriticalSection();
	};
}

#endif /* INCLUDE_UTILITY_TIMER_TIMEUNIT_H_ */
