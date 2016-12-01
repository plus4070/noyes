#ifndef CONDITION_H_
#define CONDITION_H_

#include "../../DCPS/Infrastructure/Type/BasicDefinitions.h"
#include "../../Utility/DoublyLinkedList.h"

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
	class CNU_DDS_DLL_API Condition
	{
	public:
		WaitSetSeq	related_waitset_list;

	public:
		bool	get_trigger_value(void);
	};

	typedef	DoublyLinkedList<Condition>	ConditionSeq;
}

#ifdef __cplusplus
}
#endif
#endif
