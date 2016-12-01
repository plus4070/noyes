#ifndef INSTANCEHANDLE_T_H_
#define INSTANCEHANDLE_T_H_

#include "BasicDefinitions.h"
#include "../../../Utility/DoublyLinkedList.h"

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
	typedef	HANDLE_TYPE_NATIVE	InstanceHandle_t;
	typedef	DoublyLinkedList<InstanceHandle_t>	InstanceHandleSeq;

	const	InstanceHandle_t	HANDLE_NIL	= HANDLE_NIL_NATIVE;
}

#ifdef __cplusplus
}
#endif
#endif
