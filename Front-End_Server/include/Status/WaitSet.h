#ifndef WAITSET_H_
#define WAITSET_H_

#include "../DCPS/Infrastructure/Type/ReturnCode_t.h"
#include "../DCPS/Infrastructure/Type/Duration_t.h"
#include "Condition/Condition.h"

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
	class CNU_DDS_DLL_API WaitSet
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

#ifdef __cplusplus
}
#endif
#endif
