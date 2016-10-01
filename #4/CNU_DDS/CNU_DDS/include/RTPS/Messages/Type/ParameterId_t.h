#ifndef PARAMETERID_T_H_
#define PARAMETERID_T_H_

namespace CNU_DDS
{
	class ParameterId_t
	{
	public:
		short	value;
	};

	const ParameterId_t	PID_RESERVED_PARAMETER_ID				= {0x0000};	//Compare with a result of (ParameterId_t::value & 0x8000)
	const ParameterId_t	PID_VENDOR_SPECIFIC_PARAMETER_ID		= {0x8000};	//Compare with a result of (ParameterId_t::value & 0x8000)
	const ParameterId_t	PID_IGNORE_PARAMETER_ID					= {0x0000};	//Compare with a result of (ParameterId_t::value & 0x4000)
	const ParameterId_t	PID_INCOMPATIBLE_QOS_PARAMETER_ID		= {0x4000};	//Compare with a result of (ParameterId_t::value & 0x4000)

	const ParameterId_t	PID_PAD									= {0x0000};
	const ParameterId_t	PID_SENTINEL							= {0x0001};
	const ParameterId_t	PID_USER_DATA							= {0x002c};
	const ParameterId_t	PID_TOPIC_NAME							= {0x0005};
	const ParameterId_t	PID_TYPE_NAME							= {0x0007};

	const ParameterId_t	PID_GROUP_DATA							= {0x002d};
	const ParameterId_t	PID_TOPIC_DATA							= {0x002e};
	const ParameterId_t	PID_DURABILITY							= {0x001d};
	const ParameterId_t	PID_DURABILITY_SERVICE					= {0x001e};
	const ParameterId_t	PID_DEADLINE							= {0x0023};

	const ParameterId_t	PID_LATENCY_BUDGET						= {0x0027};
	const ParameterId_t	PID_LIVELINESS							= {0x001b};
	const ParameterId_t	PID_RELIABILITY							= {0x001a};
	const ParameterId_t	PID_LIFESPAN							= {0x002b};
	const ParameterId_t	PID_DESTINATION_ORDER					= {0x0025};

	const ParameterId_t	PID_HISTORY								= {0x0040};
	const ParameterId_t	PID_RESOURCE_LIMITS						= {0x0041};
	const ParameterId_t	PID_OWNERSHIP							= {0x001f};
	const ParameterId_t	PID_OWNERSHIP_STRENGTH					= {0x0006};
	const ParameterId_t	PID_PRESENTATION						= {0x0021};

	const ParameterId_t	PID_PARTITION							= {0x0029};
	const ParameterId_t	PID_TIME_BASED_FILTER					= {0x0004};
	const ParameterId_t	PID_TRANSPORT_PRIORITY					= {0x0049};
	const ParameterId_t	PID_PROTOCOL_VERSION					= {0x0015};
	const ParameterId_t	PID_VENDORID							= {0x0016};

	const ParameterId_t	PID_UNICAST_LOCATOR						= {0x002f};
	const ParameterId_t	PID_MULTICAST_LOCATOR					= {0x0030};
	const ParameterId_t	PID_MULTICAST_IPADDRESS					= {0x0011};
	const ParameterId_t	PID_DEFAULT_UNICAST_LOCATOR				= {0x0031};
	const ParameterId_t	PID_DEFAULT_MULTICAST_LOCATOR			= {0x0048};

	const ParameterId_t	PID_METATRAFFIC_UNICAST_LOCATOR			= {0x0032};
	const ParameterId_t	PID_METATRAFFIC_MULTICAST_LOCATOR		= {0x0033};
	const ParameterId_t	PID_DEFAULT_UNICAST_IPADDRESS			= {0x000c};
	const ParameterId_t	PID_DEFAULT_UNICAST_PORT				= {0x000e};
	const ParameterId_t	PID_METATRAFFIC_UNICAST_IPADDRESS		= {0x0045};

	const ParameterId_t	PID_METATRAFFIC_UNICAST_PORT			= {0x000d};
	const ParameterId_t	PID_METATRAFFIC_MULTICAST_IPADDRESS		= {0x000b};
	const ParameterId_t	PID_METATRAFFIC_MULTICAST_PORT			= {0x0046};
	const ParameterId_t	PID_EXPECTS_INLINE_QOS					= {0x0043};
	const ParameterId_t	PID_PARTICIPANT_MANUAL_LIVELINESS_COUNT	= {0x0034};

	const ParameterId_t	PID_PARTICIPANT_BUILTIN_ENDPOINTS		= {0x0044};
	const ParameterId_t	PID_PARTICIPANT_LEASE_DURATION			= {0x0002};
	const ParameterId_t	PID_CONTENT_FILTER_PROPERTY				= {0x0035};
	const ParameterId_t	PID_PARTICIPANT_GUID					= {0x0050};
	const ParameterId_t	PID_PARTICIPANT_ENDPOINT				= {0x0051};

	const ParameterId_t	PID_GROUP_GUID							= {0x0052};
	const ParameterId_t	PID_GROUP_ENTITYID						= {0x0053};
	const ParameterId_t	PID_BUILTIN_ENDPOINT_SET				= {0x0058};
	const ParameterId_t	PID_PROPERTY_LIST						= {0x0059};
	const ParameterId_t	PID_TYPE_MAX_SIZE_SERIALIZED			= {0x0060};

	const ParameterId_t	PID_ENTITY_NAME							= {0x0062};
	const ParameterId_t	PID_KEY_HASH							= {0x0070};
	const ParameterId_t	PID_STATUS_INFO							= {0x0071};

	const ParameterId_t	PID_ENDPOINT_GUID						= {0x005a};
}

#endif
