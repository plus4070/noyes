#ifndef INCLUDE_UTILITY_NETWORK_TRANSCEIVERINFO_H_
#define INCLUDE_UTILITY_NETWORK_TRANSCEIVERINFO_H_

#include "Transceiver.h"

namespace CNU_DDS
{
	class TransceiverInfo
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


#endif /* INCLUDE_UTILITY_NETWORK_TRANSCEIVERINFO_H_ */
