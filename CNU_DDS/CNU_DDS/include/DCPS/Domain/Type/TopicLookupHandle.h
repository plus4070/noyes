#ifndef TOPICLOOKUPHANDLE_H_
#define TOPICLOOKUPHANDLE_H_

#include <iostream>

#include "../../../Utility/PlatformOrientedInterface.h"
#include "../../../Utility/DoublyLinkedList.h"

#include "../../Infrastructure/Type/BasicDefinitions.h"

namespace CNU_DDS
{
	class TopicLookupHandle
	{
	public:
		std::string				topic_name;
		CONDITION_TYPE_NATIVE*	condition;
	};

	typedef	DoublyLinkedList<TopicLookupHandle>	TopicLookupHandleSeq;
}

#endif
