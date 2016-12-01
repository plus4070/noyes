#ifndef STATUSKIND_H_
#define STATUSKIND_H_

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
	typedef	unsigned long	StatusKind;
	typedef	unsigned long	StatusMask;	// bit-mask StatusKind

	const	StatusKind	INCONSISTENT_TOPIC_STATUS			= 0x0001 << 0;
	const	StatusKind	OFFERED_DEADLINE_MISSED_STATUS		= 0x0001 << 1;
	const	StatusKind	REQUESTED_DEADLINE_MISSED_STATUS	= 0x0001 << 2;
	const	StatusKind	OFFERED_INCOMPATIBLE_QOS_STATUS		= 0x0001 << 5;
	const	StatusKind	REQUESTED_INCOMPATIBLE_QOS_STATUS	= 0x0001 << 6;
	const	StatusKind	SAMPLE_LOST_STATUS					= 0x0001 << 7;
	const	StatusKind	SAMPLE_REJECTED_STATUS				= 0x0001 << 8;
	const	StatusKind	DATA_ON_READERS_STATUS				= 0x0001 << 9;
	const	StatusKind	DATA_AVAILABLE_STATUS				= 0x0001 << 10;
	const	StatusKind	LIVELINESS_LOST_STATUS				= 0x0001 << 11;
	const	StatusKind	LIVELINESS_CHANGED_STATUS			= 0x0001 << 12;
	const	StatusKind	PUBLICATION_MATCHED_STATUS			= 0x0001 << 13;
	const	StatusKind	SUBSCRIPTION_MATCHED_STATUS			= 0x0001 << 14;
}

#ifdef __cplusplus
}
#endif
#endif
