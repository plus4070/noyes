#ifndef DURABILITYSERVICEQOSPOLICY_H_
#define DURABILITYSERVICEQOSPOLICY_H_

#include "QosPolicy.h"
#include "HistoryQosPolicy.h"
#include "../../DCPS/Infrastructure/Type/Duration_t.h"

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
	class CNU_DDS_DLL_API DurabilityServiceQosPolicy : public QosPolicy
	{
	public:
		Duration_t				service_cleanup_delay;
		HistoryQosPolicyKind	history_kind;
		long					history_depth;
		long					max_samples;
		long					max_instances;
		long					max_samples_per_instance;

	public:
		DurabilityServiceQosPolicy(void);
		~DurabilityServiceQosPolicy(void);
	};
}

#ifdef __cplusplus
}
#endif
#endif
