#ifndef PROTOCOLVERSION_T_H_
#define PROTOCOLVERSION_T_H_

#ifdef CNU_DDS_DLL
#define CNU_DDS_DLL_API	__declspec(dllexport)	
#else
#define CNU_DDS_DLL_API	__declspec(dllimport)
#endif
#ifdef __cplusplus
extern "C" {
#endif
namespace CNU_DDS
{
	class CNU_DDS_DLL_API ProtocolVersion_t
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

#ifdef __cplusplus
}
#endif
#endif
