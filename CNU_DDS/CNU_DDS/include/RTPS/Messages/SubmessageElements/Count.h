#ifndef COUNT_H_
#define COUNT_H_

#include "SubmessageElement.h"

#include "../Type/Count_t.h"

namespace CNU_DDS
{
	class Count : public SubmessageElement
	{
	public:
		Count_t	value;
	};
}

#endif
