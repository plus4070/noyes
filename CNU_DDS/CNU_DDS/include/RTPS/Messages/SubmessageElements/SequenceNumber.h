#ifndef SEQUENCENUMBER_H_
#define SEQUENCENUMBER_H_

#include "SubmessageElement.h"

#include "../../Structure/Type/SequenceNumber_t.h"

namespace CNU_DDS
{
	class SequenceNumber : public SubmessageElement
	{
	public:
		SequenceNumber_t	value;
	};
}

#endif
