#ifndef GUARDCONDITION_H_
#define GUARDCONDITION_H_

#include "Condition.h"
#include "../../DCPS/Infrastructure/Type/ReturnCode_t.h"

namespace CNU_DDS
{
	class GuardCondition : public Condition
	{
	public:
		ReturnCode_t	set_trigger_value(bool value);
	};
}

#endif
