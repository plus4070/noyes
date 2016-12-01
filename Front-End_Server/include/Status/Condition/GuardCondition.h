#ifndef GUARDCONDITION_H_
#define GUARDCONDITION_H_

#include "Condition.h"
#include "../../DCPS/Infrastructure/Type/ReturnCode_t.h"

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
	class CNU_DDS_DLL_API GuardCondition : public Condition
	{
	public:
		ReturnCode_t	set_trigger_value(bool value);
	};
}

#ifdef __cplusplus
}
#endif
#endif
