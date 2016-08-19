#ifndef TIMESTAMP_H_
#define TIMESTAMP_H_

#include "SubmessageElement.h"

#include "../Type/Time_t.h"

namespace CNU_DDS
{
	class Timestamp : public SubmessageElement
	{
	public:
		Time_t	value;
	};
}

#endif
