#ifndef GUIDPREFIX_H_
#define GUIDPREFIX_H_

#include "SubmessageElement.h"

#include "../../Structure/Type/GuidPrefix_t.h"

namespace CNU_DDS
{
	class GuidPrefix : public SubmessageElement
	{
	public:
		GuidPrefix_t	value;
	};
}

#endif
