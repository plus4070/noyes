#ifndef SERIALIZEDDATAFRAGMENT_H_
#define SERIALIZEDDATAFRAGMENT_H_

#include "SubmessageElement.h"

namespace CNU_DDS
{
	class SerializedDataFrag: public SubmessageElement
	{
	public:
		char*	value;
	};
}

#endif
