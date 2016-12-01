#ifndef STATUSCONDITION_H_
#define STATUSCONDITION_H_

#include "Condition.h"
#include "../../DCPS/Infrastructure/Type/BasicDefinitions.h"
#include "../../DCPS/Infrastructure/Type/ReturnCode_t.h"
#include "../Type/StatusKind.h"

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
	class CNU_DDS_DLL_API StatusCondition : public Condition
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

#ifdef __cplusplus
}
#endif
#endif
