#ifndef ENTITYID_H_
#define ENTITYID_H_

#include "SubmessageElement.h"

#include "../../Structure/Type/EntityId_t.h"

namespace CNU_DDS
{
	class EntityId : public SubmessageElement
	{
	public:
		EntityId_t	value;
	};
}

#endif
