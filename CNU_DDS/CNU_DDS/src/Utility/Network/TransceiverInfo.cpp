#include "../../../include/Utility/Network/TransceiverInfo.h"

namespace CNU_DDS
{
	TransceiverInfo::TransceiverInfo()
	{
		transceiver	= 0;
		tid			= 0;
	}

	TransceiverInfo::~TransceiverInfo()
	{

	}

	bool	TransceiverInfo::operator	==	( const TransceiverInfo rvalue )
	{
		return tid == rvalue.tid;
	}

	bool	TransceiverInfo::operator	!=	( const TransceiverInfo rvalue )
	{
		return tid != rvalue.tid;
	}

	bool	TransceiverInfo::operator	>	( const TransceiverInfo rvalue )
	{
		return tid > rvalue.tid;
	}

	bool	TransceiverInfo::operator	<	( const TransceiverInfo rvalue )
	{
		return tid < rvalue.tid;
	}

	bool	TransceiverInfo::operator	>=	( const TransceiverInfo rvalue )
	{
		return tid >= rvalue.tid;
	}

	bool	TransceiverInfo::operator	<=	( const TransceiverInfo rvalue )
	{
		return tid <= rvalue.tid;
	}
}
