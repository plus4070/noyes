#include "../../../../include/RTPS/Structure/Type/GuidPrefix_t.h"
#include <stdlib.h>
#include <string.h>

namespace CNU_DDS
{
	void	GuidPrefix_t::operator	= (const GuidPrefix_t& arg)
	{
		memcpy(this, &arg, 12);
	}

	bool	GuidPrefix_t::operator	== (const GuidPrefix_t& arg)
	{
		return !memcmp(this, &arg, 12);
	}
}
