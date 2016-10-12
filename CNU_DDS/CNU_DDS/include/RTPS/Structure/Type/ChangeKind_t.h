#ifndef CHANGEKIND_T_H_
#define CHANGEKIND_T_H_

namespace CNU_DDS
{
	typedef	unsigned long	ChangeKind_t;

	const ChangeKind_t	CHANGE_KIND_ALIVE					= 0x01;
	const ChangeKind_t	CHANGE_KIND_NOT_AVLIE_DISPOSED		= 0x02;
	const ChangeKind_t	CHANGE_KIND_NOT_ALIVE_UNREGISTERED	= 0x04;
}

#endif
