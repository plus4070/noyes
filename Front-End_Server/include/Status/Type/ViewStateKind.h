#ifndef VIEWSTATEKIND_H_
#define VIEWSTATEKIND_H_

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
	typedef	unsigned long	ViewStateKind;
	typedef	unsigned long	ViewStateMask;	// bit-mask ViewStateKind

	const	ViewStateKind	NEW_VIEW_STATE		= 0x0001 << 0;
	const	ViewStateKind	NOT_NEW_VIEW_STATE	= 0x0001 << 1;

	const	ViewStateMask	ANY_VIEW_STATE		= 0xFFFF;
}

#ifdef __cplusplus
}
#endif
#endif
