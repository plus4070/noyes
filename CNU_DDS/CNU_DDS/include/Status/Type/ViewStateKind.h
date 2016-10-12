#ifndef VIEWSTATEKIND_H_
#define VIEWSTATEKIND_H_

namespace CNU_DDS
{
	typedef	unsigned long	ViewStateKind;
	typedef	unsigned long	ViewStateMask;	// bit-mask ViewStateKind

	const	ViewStateKind	NEW_VIEW_STATE		= 0x0001 << 0;
	const	ViewStateKind	NOT_NEW_VIEW_STATE	= 0x0001 << 1;

	const	ViewStateMask	ANY_VIEW_STATE		= 0xFFFF;
}

#endif
