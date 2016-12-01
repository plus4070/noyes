#ifndef DATAWRITERSTATUS_H_
#define DATAWRITERSTATUS_H_

#include "EntityStatus.h"

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
	class CNU_DDS_DLL_API DataWriterStatus : public EntityStatus
	{
	public:
		OfferedDeadlineMissedStatus		offered_deadline_missed;
		OfferedIncompatibleQosStatus	offered_incompatible_qos;
		LivelinessLostStatus			liveliness_lost;
		PublicationMatchedStatus		publication_matched;
	};
}

#ifdef __cplusplus
}
#endif
#endif
