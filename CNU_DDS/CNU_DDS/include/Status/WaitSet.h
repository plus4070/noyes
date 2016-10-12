#ifndef WAITSET_H_
#define WAITSET_H_

#include "../DCPS/Infrastructure/Type/ReturnCode_t.h"
#include "../DCPS/Infrastructure/Type/Duration_t.h"
#include "Condition/Condition.h"

namespace CNU_DDS
{
	class WaitSet
	{
	private:
		ConditionSeq	registered_condition_list;

	public:
		ReturnCode_t	attach_condition(Condition* a_condition);
		ReturnCode_t	detach_condition(Condition* a_condition);
		ReturnCode_t	wait(ConditionSeq* active_conditions, Duration_t timeout);
		ReturnCode_t	get_conditions(ConditionSeq* attached_conditions);
	};
}

#endif
