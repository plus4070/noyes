#include "../../../../include/RTPS/Messages/Type/ProtocolId_t.h"
#include <string.h>

namespace CNU_DDS
{
	bool	ProtocolId_t::operator == (const ProtocolId_t& arg)
	{
		return !(memcmp(value, arg.value, sizeof(ProtocolId_t)));
	}

	void	ProtocolId_t::operator =  (const ProtocolId_t& arg)
	{
		memcpy(value, arg.value, sizeof(ProtocolId_t));
	}
}
