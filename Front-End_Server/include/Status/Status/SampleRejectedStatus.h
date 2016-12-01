#ifndef SAMPLEREJECTEDSTATUS_H_
#define SAMPLEREJECTEDSTATUS_H_

#include "Status.h"
#include "../../DCPS/Infrastructure/Type/InstanceHandle_t.h"

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
	enum SampleRejectedStatusKind
	{
		NOT_REJECTED,
		REJECTED_BY_INSTANCES_LIMIT,
		REJECTED_BY_SAMPLES_LIMIT,
		REJECTED_BY_SMAPLES_PER_INSTANCE_LIMIT
	};

	class CNU_DDS_DLL_API SampleRejectedStatus:public Status
	{
	public:
		long						total_count;
		long						total_count_change;
		SampleRejectedStatusKind	last_reason;
		InstanceHandle_t			last_instance_handle;
	};
}

#ifdef __cplusplus
}
#endif
#endif
