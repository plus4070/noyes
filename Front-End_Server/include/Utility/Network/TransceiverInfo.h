#ifndef INCLUDE_UTILITY_NETWORK_TRANSCEIVERINFO_H_
#define INCLUDE_UTILITY_NETWORK_TRANSCEIVERINFO_H_

#include "Transceiver.h"

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
	class CNU_DDS_DLL_API TransceiverInfo
	{
	public:
		Transceiver*	transceiver;
		unsigned long	tid;			//Transceiver Identification Number

	public:
		TransceiverInfo();
		~TransceiverInfo();

		bool	operator	==	( const TransceiverInfo rvalue );
		bool	operator	!=	( const TransceiverInfo rvalue );
		bool	operator	>	( const TransceiverInfo rvalue );
		bool	operator	<	( const TransceiverInfo rvalue );
		bool	operator	>=	( const TransceiverInfo rvalue );
		bool	operator	<=	( const TransceiverInfo rvalue );
	};
}

#ifdef __cplusplus
}
#endif
#endif /* INCLUDE_UTILITY_NETWORK_TRANSCEIVERINFO_H_ */
