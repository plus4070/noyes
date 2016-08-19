#ifndef PROTOCOLVERSION_T_H_
#define PROTOCOLVERSION_T_H_

namespace CNU_DDS
{
	class ProtocolVersion_t
	{
	public:
		char	major;
		char	minor;
	};

	const ProtocolVersion_t	PROTOCOL_VERSION_1_0	= {1, 0};
	const ProtocolVersion_t	PROTOCOL_VERSION_1_1	= {1, 1};
	const ProtocolVersion_t	PROTOCOL_VERSION_2_0	= {2, 0};
	const ProtocolVersion_t	PROTOCOL_VERSION_2_1	= {2, 1};

	const ProtocolVersion_t	PROTOCOL_VERSION		= PROTOCOL_VERSION_2_1;
}

#endif
