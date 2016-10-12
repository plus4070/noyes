#ifndef INSTANCESTATEKIND_H_
#define INSTANCESTATEKIND_H_

namespace CNU_DDS
{
	typedef	unsigned long	InstanceStateKind;
	typedef	unsigned long	InstanceStateMask;	// bit-map InstanceStateKind

	const	InstanceStateKind	ALIVE_INSTANCE_STATE				= 0x0001 << 0;
	const	InstanceStateKind	NOT_ALIVE_DISPOSED_INSTANCE_STATE	= 0x0001 << 1;
	const	InstanceStateKind	NOT_ALIVE_NO_WRITERS_INSTANCE_STATE	= 0x0001 << 2;

	const	InstanceStateMask	ANY_INSTANCE_STATE			= 0xFFFF;
	const	InstanceStateMask	NOT_ALIVE_INSTANCE_STATE	= 0x0006;
}

#endif
