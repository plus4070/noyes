#ifndef READCONDITION_H_
#define READCONDITION_H_

#include "Condition.h"

#include "../../DCPS/Infrastructure/Type/BasicDefinitions.h"

#include "../Type/SampleStateKind.h"
#include "../Type/ViewStateKind.h"
#include "../Type/InstanceStateKind.h"

namespace CNU_DDS
{
	class ReadCondition : public Condition
	{
	public:
		ViewStateMask		view_state_mask;
		InstanceStateMask	instance_state_mask;
		SampleStateMask		sample_state_mask;

	public:
		pDataReader			get_datareader(void);
		SampleStateMask		get_sample_state_mask(void);
		ViewStateMask		get_view_state_mask(void);
		InstanceStateMask	get_instance_state_mask(void);
	};

	typedef DoublyLinkedList<ReadCondition>	ReadConditionSeq;
}

#endif
