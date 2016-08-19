#ifndef INSTANCEHANDLE_T_H_
#define INSTANCEHANDLE_T_H_

#include "BasicDefinitions.h"
#include "../../../Utility/DoublyLinkedList.h"

namespace CNU_DDS
{
	typedef	HANDLE_TYPE_NATIVE	InstanceHandle_t;
	typedef	DoublyLinkedList<InstanceHandle_t>	InstanceHandleSeq;

	const	InstanceHandle_t	HANDLE_NIL	= HANDLE_NIL_NATIVE;
}

#endif
