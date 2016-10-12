#ifndef PARAMETERLIST_H_
#define PARAMETERLIST_H_

#include "SubmessageElement.h"
#include "Parameter.h"

namespace CNU_DDS
{
	class ParameterList : public SubmessageElement
	{
	public:
		ParameterSeq	parameters;
	};
}

#endif
