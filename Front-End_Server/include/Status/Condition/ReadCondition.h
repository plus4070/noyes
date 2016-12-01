#ifndef READCONDITION_H_
#define READCONDITION_H_

#include "Condition.h"

#include "../../DCPS/Infrastructure/Type/BasicDefinitions.h"

#include "../Type/SampleStateKind.h"
#include "../Type/ViewStateKind.h"
#include "../Type/InstanceStateKind.h"

#ifdef CNU_DDS_DLL
#define CNU_DDS_DLL_API	__declspec(dllexport)	
#else
#define CNU_DDS_DLL_API	__declspec(dllimport)
#endif
#ifdef __cplusplus
extern "C" {
#endif
namespace CNU_DDS
{
	class CNU_DDS_DLL_API ReadCondition : public Condition
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

#ifdef __cplusplus
}
#endif
#endif
