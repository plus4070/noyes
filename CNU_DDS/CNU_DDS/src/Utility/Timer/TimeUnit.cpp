#include "../../../include/Utility/Timer/TimeUnit.h"

namespace CNU_DDS
{
	TimeUnit::TimeUnit(unsigned long a_Expire)
	{
		function		= NULL;
		data			= 0;
		this->expires	= a_Expire;
		create_mutex(&mutex);
	}

	TimeUnit::~TimeUnit()
	{
		destroy_mutex(&mutex);
	}

	void				TimeUnit::setExpireTime(unsigned long a_Expire)
	{
		this->expires	= a_Expire;
	}

	void				TimeUnit::setDataValue(unsigned long a_Value)
	{
		this->data	= a_Value;
	}

	void				TimeUnit::setExecuteFunction(void (*a_function)(unsigned long))
	{
		this->function	= a_function;
	}

	unsigned long		TimeUnit::getExpireTime()
	{
		return expires;
	}

	unsigned long		TimeUnit::getDataValue()
	{
		return data;
	}

	void				TimeUnit::execute(unsigned long arg)
	{
		function(arg);
	}

	void				TimeUnit::entryCriticalSection()
	{
		mutex_lock(&mutex);
	}

	void				TimeUnit::leaveCriticalSection()
	{
		mutex_unlock(&mutex);
	}
}
