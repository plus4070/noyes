#ifndef LOCATORLIST_H_
#define LOCATORLIST_H_

#include "SubmessageElement.h"

#include "../../Structure/Type/Locator_t.h"

namespace CNU_DDS
{
	class LocatorList : public SubmessageElement
	{
	public:
		LocatorSeq	value;
	};
}

#endif
