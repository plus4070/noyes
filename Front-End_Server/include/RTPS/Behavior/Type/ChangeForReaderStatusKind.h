#ifndef CHANGEFORREADERSTATUSKIND_H_
#define CHANGEFORREADERSTATUSKIND_H_

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
	typedef unsigned long	ChangeForReaderStatusKind;

	const ChangeForReaderStatusKind	CHANGE_FOR_READER_STATUS_KIND_UNSENT			= 0x01;
	const ChangeForReaderStatusKind	CHANGE_FOR_READER_STATUS_KIND_UNACKNOWLEDGED	= 0x02;
	const ChangeForReaderStatusKind	CHANGE_FOR_READER_STATUS_KIND_REQUESTED			= 0x04;
	const ChangeForReaderStatusKind	CHANGE_FOR_READER_STATUS_KIND_ACKNOWLEDGED		= 0x08;
	const ChangeForReaderStatusKind	CHANGE_FOR_READER_STATUS_KIND_UNDERWAY			= 0x10;
}

#ifdef __cplusplus
}
#endif
#endif
