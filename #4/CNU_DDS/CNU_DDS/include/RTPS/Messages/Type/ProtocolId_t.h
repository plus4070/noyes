#ifndef PROTOCOLID_T_H_
#define PROTOCOLID_T_H_

namespace CNU_DDS
{
	class ProtocolId_t
	{
	public:
		char	value[4];

	public:
		bool	operator == (const ProtocolId_t& arg);
		void	operator =  (const ProtocolId_t& arg);
	};

	const ProtocolId_t	PROTOCOL_RTPS	= {'R', 'T', 'P', 'S'};
}

#endif
