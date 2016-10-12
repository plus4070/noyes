#include "../../../../include/RTPS/Structure/Type/GUID_t.h"
#include <string.h>

namespace CNU_DDS
{
	void	GUID_t::operator	= (const GUID_t& arg)
	{
		memcpy(this, &arg, sizeof(GUID_t));
	}

	bool	GUID_t::operator	== (const GUID_t& arg)
	{
		return !memcmp(this, &arg, sizeof(GUID_t));
	}
}
