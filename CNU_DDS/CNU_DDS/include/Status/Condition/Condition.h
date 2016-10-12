#ifndef CONDITION_H_
#define CONDITION_H_

#include "../../DCPS/Infrastructure/Type/BasicDefinitions.h"
#include "../../Utility/DoublyLinkedList.h"

namespace CNU_DDS
{
	class Condition
	{
	public:
		WaitSetSeq	related_waitset_list;

	public:
		bool	get_trigger_value(void);
	};

	typedef	DoublyLinkedList<Condition>	ConditionSeq;
}

#endif
