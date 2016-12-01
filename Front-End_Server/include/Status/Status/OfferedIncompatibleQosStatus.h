#ifndef OFFEREDINCOMPATIBLEQOSSTATUS_H_
#define OFFEREDINCOMPATIBLEQOSSTATUS_H_

#include "Status.h"
#include "QosPolicyCount.h"

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
	class CNU_DDS_DLL_API OfferedIncompatibleQosStatus:public Status
	{
	public:
		long				total_count;
		long				total_count_change;
		QosPolicyId_t		last_policy_id;
		QosPolicyCountSeq	policies;
	};
}

#ifdef __cplusplus
}
#endif
#endif
