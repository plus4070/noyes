#ifndef FRAGMENTNUMBERSET_H_
#define FRAGMENTNUMBERSET_H_

#include "SubmessageElement.h"

#include "../Type/FragmentNumber_t.h"

namespace CNU_DDS
{
	class FragmentNumberSet : public SubmessageElement
	{
	public:
		FragmentNumber_t	base;
		unsigned long		numBits;
		FragmentNumberSeq	set;
	};
}

#endif
