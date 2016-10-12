#ifndef PROTOCOLVERSION_H_
#define PROTOCOLVERSION_H_

#include "SubmessageElement.h"
#include "../../Structure/Type/ProtocolVersion_t.h"

namespace CNU_DDS
{
	class ProtocolVersion : public SubmessageElement
	{
	public:
		ProtocolVersion_t	value;
	};
}

#endif
