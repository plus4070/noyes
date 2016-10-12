#ifndef CHANGEFROMWRITERSTATUSKIND_H_
#define CHANGEFROMWRITERSTATUSKIND_H_

namespace CNU_DDS
{
	typedef unsigned long	ChangeFromWriterStatusKind;

	const ChangeFromWriterStatusKind	CHANGE_FROM_WRITER_STATUS_KIND_LOST		= 0x01;
	const ChangeFromWriterStatusKind	CHANGE_FROM_WRITER_STATUS_KIND_MISSING	= 0x02;
	const ChangeFromWriterStatusKind	CHANGE_FROM_WRITER_STATUS_KIND_RECEIVED	= 0x04;
	const ChangeFromWriterStatusKind	CHANGE_FROM_WRITER_STATUS_KIND_UNKNOWN	= 0x08;
}

#endif
