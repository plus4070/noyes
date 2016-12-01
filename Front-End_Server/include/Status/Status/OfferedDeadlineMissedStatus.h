#ifndef OFFEREDDEADLINEMISSEDSTATUS_H_
#define OFFEREDDEADLINEMISSEDSTATUS_H_

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
	class CNU_DDS_DLL_API OfferedDeadlineMissedStatus:public Status
	{
	public:
		long				total_count;
		long				total_count_change;
		InstanceHandle_t	last_instance_handle;
	};
}

#ifdef __cplusplus
}
#endif
#endif
