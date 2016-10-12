#ifndef SEQUENCENUMBERSET_H_
#define SEQUENCENUMBERSET_H_

#include "SubmessageElement.h"

#include "../../Structure/Type/SequenceNumber_t.h"

namespace CNU_DDS
{
	class SequenceNumberSet : public SubmessageElement
	{
	public:
		SequenceNumber_t	base;
		unsigned long		numBits;
		SequenceNumberSeq	set;
	};
}

#endif
