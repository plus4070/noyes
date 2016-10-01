#ifndef SAMPLESTATEKIND_H_
#define SAMPLESTATEKIND_H_

namespace CNU_DDS
{
	typedef	unsigned long	SampleStateKind;
	typedef	unsigned long	SampleStateMask;	// bit-mask SampleStateKind

	const	SampleStateKind	READ_SAMPLE_STATE		= 0x0001 << 0;
	const	SampleStateKind	NOT_READ_SAMPLE_STATE	= 0x0001 << 1;

	const	SampleStateMask	ANY_SAMPLE_STATE		= 0xFFFF;
}

#endif
