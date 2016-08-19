#ifndef HEADER_H_
#define HEADER_H_

#include "Type/ProtocolId_t.h"

#include "../Structure/Type/ProtocolVersion_t.h"
#include "../Structure/Type/VendorId_t.h"
#include "../Structure/Type/GuidPrefix_t.h"

namespace CNU_DDS
{
	class Header
	{
	public:
		ProtocolId_t		protocol;
		ProtocolVersion_t	version;
		VendorId_t			vendor_id;
		GuidPrefix_t		guid_prefix;
	};
}

#endif
