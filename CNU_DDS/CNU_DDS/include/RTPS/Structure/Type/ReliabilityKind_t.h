#ifndef RELIABILITYKIND_T_H_
#define RELIABILITYKIND_T_H_

namespace CNU_DDS
{
	typedef	unsigned long	ReliabilityKind_t;

	const ReliabilityKind_t	RELIABILITY_KIND_BEST_EFFORT	= 0x01;
	const ReliabilityKind_t	RELIABILITY_KIND_RELIABLE		= 0x03;
}

#endif
