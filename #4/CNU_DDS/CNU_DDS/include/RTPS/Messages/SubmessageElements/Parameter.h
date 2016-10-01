#ifndef PARAMETER_H_
#define PARAMETER_H_

#include "SubmessageElement.h"

#include "../Type/ParameterId_t.h"

#include "../../../Utility/DoublyLinkedList.h"

namespace CNU_DDS
{
	class Parameter : public SubmessageElement
	{
	public:
		ParameterId_t	parameter_id;
		short			length;
		char*			value;
	};

	typedef DoublyLinkedList<Parameter>	ParameterSeq;
}

#endif
