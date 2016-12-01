#ifndef PROTOCOLID_T_H_
#define PROTOCOLID_T_H_

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
	class CNU_DDS_DLL_API ProtocolId_t
	{
	public:
		char	value[4];

	public:
		bool	operator == (const ProtocolId_t& arg);
		void	operator =  (const ProtocolId_t& arg);
	};

	const ProtocolId_t	PROTOCOL_RTPS	= {'R', 'T', 'P', 'S'};
}

#ifdef __cplusplus
}
#endif
#endif
