#ifndef SAMPLESTATEKIND_H_
#define SAMPLESTATEKIND_H_

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
	typedef	unsigned long	SampleStateKind;
	typedef	unsigned long	SampleStateMask;	// bit-mask SampleStateKind

	const	SampleStateKind	READ_SAMPLE_STATE		= 0x0001 << 0;
	const	SampleStateKind	NOT_READ_SAMPLE_STATE	= 0x0001 << 1;

	const	SampleStateMask	ANY_SAMPLE_STATE		= 0xFFFF;
}

#ifdef __cplusplus
}
#endif
#endif
