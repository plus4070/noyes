#ifndef STATUSCONDITION_H_
#define STATUSCONDITION_H_

#include "Condition.h"
#include "../../DCPS/Infrastructure/Type/BasicDefinitions.h"
#include "../../DCPS/Infrastructure/Type/ReturnCode_t.h"
#include "../Type/StatusKind.h"

namespace CNU_DDS
{
	class StatusCondition : public Condition
	{
	public:
		pEntity			related_entity;
		StatusMask		enabled_statuses;

	public:
		ReturnCode_t	set_enabled_statuses(StatusMask mask);
		StatusMask		get_enabled_statuses(void);
		pEntity			get_entity(void);
	};

	typedef DoublyLinkedList<StatusCondition>	StatusConditionSeq;
}

#endif
