#ifndef TOPICLOOKUPHANDLE_H_
#define TOPICLOOKUPHANDLE_H_

#include <iostream>

#include "../../../Utility/PlatformOrientedInterface.h"
#include "../../../Utility/DoublyLinkedList.h"

#include "../../Infrastructure/Type/BasicDefinitions.h"

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
	class CNU_DDS_DLL_API TopicLookupHandle
	{
	public:
		std::string				topic_name;
		CONDITION_TYPE_NATIVE*	condition;
	};

	typedef	DoublyLinkedList<TopicLookupHandle>	TopicLookupHandleSeq;
}

#ifdef __cplusplus
}
#endif
#endif
