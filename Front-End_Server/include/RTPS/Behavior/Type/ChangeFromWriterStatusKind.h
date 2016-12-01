#ifndef CHANGEFROMWRITERSTATUSKIND_H_
#define CHANGEFROMWRITERSTATUSKIND_H_

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
	typedef unsigned long	ChangeFromWriterStatusKind;

	const ChangeFromWriterStatusKind	CHANGE_FROM_WRITER_STATUS_KIND_LOST		= 0x01;
	const ChangeFromWriterStatusKind	CHANGE_FROM_WRITER_STATUS_KIND_MISSING	= 0x02;
	const ChangeFromWriterStatusKind	CHANGE_FROM_WRITER_STATUS_KIND_RECEIVED	= 0x04;
	const ChangeFromWriterStatusKind	CHANGE_FROM_WRITER_STATUS_KIND_UNKNOWN	= 0x08;
}

#ifdef __cplusplus
}
#endif
#endif
