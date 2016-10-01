#include "../../../../include/RTPS/Structure/Type/EntityId_t.h"
#include <string.h>

namespace CNU_DDS
{
	void	EntityId_t::operator =  (const EntityId_t& arg)
	{
		memcpy(this, &arg, sizeof(EntityId_t));
	}

	bool	EntityId_t::operator == (const EntityId_t& arg)
	{
		return !memcmp(this, &arg, sizeof(EntityId_t));
	}
}
