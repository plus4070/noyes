#ifndef PARTICIPANTPROXY_H_
#define PARTICIPANTPROXY_H_

#include "../Structure/Type/ProtocolVersion_t.h"
#include "../Structure/Type/GuidPrefix_t.h"
#include "../Structure/Type/VendorId_t.h"
#include "../Structure/Type/Locator_t.h"

#include "../Messages/Type/Count_t.h"

#include "Type/BuiltinEndpointSet_t.h"

namespace CNU_DDS
{
	class ParticipantProxy
	{
	public:
		ProtocolVersion_t		protocol_version;
		GuidPrefix_t			guid_prefix;
		VendorId_t				vendor_id;
		bool					expects_inline_qos;
		BuiltinEndpointSet_t	available_builtin_endpoints;
		LocatorSeq*				metatraffic_unicast_locator_list;
		LocatorSeq*				metatraffic_multicast_locator_list;
		LocatorSeq*				default_unicast_locator_list;
		LocatorSeq*				default_multicast_locator_list;
		Count_t					manual_liveliness_count;
	};

	typedef DoublyLinkedList<ParticipantProxy>	ParticipantProxySeq;
}

#endif
