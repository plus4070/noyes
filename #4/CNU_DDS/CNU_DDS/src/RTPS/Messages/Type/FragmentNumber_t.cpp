#include "../../../../include/RTPS/Messages/Type/FragmentNumber_t.h"

namespace CNU_DDS
{
	FragmentNumber_t	FragmentNumber_t::operator	+ (const FragmentNumber_t& arg)
	{
		FragmentNumber_t	result	= {0};
		result.value	= this->value	+ arg.value;
		return result;
	}

	FragmentNumber_t	FragmentNumber_t::operator	- (const FragmentNumber_t& arg)
	{
		FragmentNumber_t	result	= {0};
		result.value	= this->value	- arg.value;
		return result;
	}

	void				FragmentNumber_t::operator	= (const FragmentNumber_t& arg)
	{
		this->value	= arg.value;
	}
}
