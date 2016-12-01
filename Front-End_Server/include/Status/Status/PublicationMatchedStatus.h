#ifndef PUBLICATIONMATCHEDSTATUS_H_
#define PUBLICATIONMATCHEDSTATUS_H_

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
	class CNU_DDS_DLL_API PublicationMatchedStatus:public Status
	{
	public:
		long				total_count;
		long				total_count_change;
		long				current_count;
		long				current_count_change;
		InstanceHandle_t	last_subscription_handle;
	};
}

#ifdef __cplusplus
}
#endif
#endif
