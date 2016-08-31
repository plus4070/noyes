#ifndef BUILTINENDPOINTSET_T_H_
#define BUILTINENDPOINTSET_T_H_

namespace CNU_DDS
{
	typedef unsigned long	BuiltinEndpointSet_t;

	const BuiltinEndpointSet_t	DISC_BUILTIN_ENDPOINT_PARTICIPANT_ANNOUNCER			= 0x00000001 << 0;
	const BuiltinEndpointSet_t	DISC_BUILTIN_ENDPOINT_PARTICIPANT_DETECTOR			= 0x00000001 << 1;
	const BuiltinEndpointSet_t	DISC_BUILTIN_ENDPOINT_PUBLICATION_ANNOUNCER			= 0x00000001 << 2;
	const BuiltinEndpointSet_t	DISC_BUILTIN_ENDPOINT_PUBLICATION_DETECTOR			= 0x00000001 << 3;
	const BuiltinEndpointSet_t	DISC_BUILTIN_ENDPOINT_SUBSCRIPTION_ANNOUNCER		= 0x00000001 << 4;
	const BuiltinEndpointSet_t	DISC_BUILTIN_ENDPOINT_SUBSCRIPTION_DETECTOR			= 0x00000001 << 5;
	const BuiltinEndpointSet_t	DISC_BUILTIN_ENDPOINT_PARTICIPANT_PROXY_ANNOUNCER	= 0x00000001 << 6;
	const BuiltinEndpointSet_t	DISC_BUILTIN_ENDPOINT_PARTICIPANT_PROXY_DETECTOR	= 0x00000001 << 7;
	const BuiltinEndpointSet_t	DISC_BUILTIN_ENDPOINT_PARTICIPANT_STATE_ANNOUNCER	= 0x00000001 << 8;
	const BuiltinEndpointSet_t	DISC_BUILTIN_ENDPOINT_PARTICIPANT_STATE_DETECTOR	= 0x00000001 << 9;
	const BuiltinEndpointSet_t	BUILTIN_ENDPOINT_PARTICIPANT_MESSAGE_DATA_WRITER	= 0x00000001 << 10;
	const BuiltinEndpointSet_t	BUILTIN_ENDPOINT_PARTICIPANT_MESSAGE_READER			= 0x00000001 << 11;
}

#endif