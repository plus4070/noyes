#ifndef FRAGMENTNUMBER_H_
#define FRAGMENTNUMBER_H_

#include "SubmessageElement.h"

#include "../Type/FragmentNumber_t.h"

namespace CNU_DDS
{
	class FragmentNumber : public SubmessageElement
	{
	public:
		FragmentNumber_t	value;

	};
}

#endif
