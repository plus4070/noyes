#ifndef DATAREADERSTATUS_H_
#define DATAREADERSTATUS_H_

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
	class CNU_DDS_DLL_API DataReaderStatus : public EntityStatus
	{
	public:
		RequestedDeadlineMissedStatus	requested_deadline_missed;
		RequestedIncompatibleQosStatus	requested_incompatile_qos;
		SampleRejectedStatus			sample_rejected;
		LivelinessChangedStatus			liveliness_changed;
		SubscriptionMatchedStatus		subscription_matched;
		SampleLostStatus				sample_lost;
	};
}

#ifdef __cplusplus
}
#endif
#endif
