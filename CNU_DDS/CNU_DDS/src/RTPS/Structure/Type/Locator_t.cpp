#include "../../../../include/RTPS/Structure/Type/Locator_t.h"
#include <string.h>

namespace CNU_DDS
{
	void			setAddress(char* a_addr, unsigned long a_port)
	{

	}

	bool			Locator_t::operator	== (const Locator_t& arg)
	{
		return !(memcmp(this, &arg, sizeof(Locator_t)));
	}

	void			Locator_t::operator	= (const Locator_t& arg)
	{
		memcpy(this, &arg, sizeof(Locator_t));
	}
}
